//*****************************************************************************
// Synchrotron SOLEIL
//
//
// Creation : 20/07/2005
// Author   : Stephane Poirier
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; version 2 of the License.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//*****************************************************************************

#include "base.h"
#include "file.h"
#include "membuf.h"
#include "nxfile.h"

#include <iostream>
#include <fstream>
#include <sstream>  
#include <vector>

#include "nexusevaluator.h"
#include "extractor.h"
#include "templateparsor.h"

using namespace gdshare;

typedef struct _LineToken
{
  pcsz pszToken1;                // Token
  TemplateToken::Type TokenType; // Token type
  bool bRemoveToken;
} SLineToken;

const int g_iTokenCount = 13;
SLineToken g_LineToken[g_iTokenCount] = 
{ 
  {"@(", TemplateToken::LOOP, true},
  {"@)", TemplateToken::END_LOOP, true},
  {"?(", TemplateToken::IF, true},
  {"?)", TemplateToken::END_IF, true},
  {"\"", TemplateToken::PRINT, false},
  {">", TemplateToken::OUTPUT, true},
  {"b>", TemplateToken::BINARY_OUTPUT, true},
  {"%", TemplateToken::SET, true},
  {"(", TemplateToken::BLOCK_START, true},
  {")", TemplateToken::BLOCK_END, true},
  {"...", TemplateToken::PADDING, true},
  {"'", TemplateToken::BINARY, false},
  {"+", TemplateToken::INCLUDE, true}
};

//-----------------------------------------------------------------------------
// TemplateFileParsor::TemplateFileParsor
//-----------------------------------------------------------------------------
TemplateFileParsor::TemplateFileParsor(VecToken *pvecToken)
                    :m_vecToken(*pvecToken), m_pVarEval(NULL)
{
  m_bHeaderParsed = false;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::SuppressComments
//-----------------------------------------------------------------------------
void TemplateFileParsor::SuppressComments(String *pstrLine)
{
  // Extract payload charge
  int iSearchPos = 0;
  while( true )
  {
    int iCom = pstrLine->find("//", iSearchPos);
    if( (uint)iCom != string::npos )
    {
      if( ((*pstrLine)[iCom - 1] == ' ' || (*pstrLine)[iCom - 1] == '\t') )
      {
        pstrLine->erase(iCom, pstrLine->size()-iCom);
          break;
      }
      iSearchPos = iCom + 1;
    }
    else
      break;
  }
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseSet
//-----------------------------------------------------------------------------
void TemplateFileParsor::ParseSet(String *pstrLine, int iLine)
{
  TemplateToken aToken;
  aToken.m_TokenType = TemplateToken::SET;

  // Looking for the commentaries to eliminate them.
  SuppressComments(pstrLine);

  // Parsing variable name
  String strVarName;
  pstrLine->ExtractToken('=' , &strVarName);

  aToken.m_strParam1 = strVarName;
  aToken.m_strParam1.Trim();
  aToken.m_strParam2 = *pstrLine; // value
  aToken.m_strParam2.Trim();
  aToken.m_iTemplateLine = iLine;

  m_vecToken.push_back(aToken);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseOutput
//-----------------------------------------------------------------------------
void TemplateFileParsor::ParseOutput(String *pstrLine, int iLine, bool bBinary)
{
  TemplateToken aToken;
  aToken.m_TokenType = TemplateToken::OUTPUT;

  // we look for the commentaries to eliminate them.
  SuppressComments(pstrLine);

  // Store parameters
  aToken.m_strParam1 = *pstrLine;
  aToken.m_strParam1.Trim();
  aToken.m_iTemplateLine = iLine;
  aToken.m_iParam1 = bBinary ? 1 : 0;

  m_vecToken.push_back(aToken);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseLoop
//-----------------------------------------------------------------------------
int TemplateFileParsor::ParseLoop(String *pstrLine, int iLine)
{
  TemplateToken aToken;
  aToken.m_iTemplateLine = iLine;
  
  // we look for the commentaries to eliminate them.
  SuppressComments(pstrLine);

  // Extract the name of iteration variable
  pstrLine->ExtractToken('=', &aToken.m_strParam1);
  // Next parameter
  int rc = pstrLine->ExtractToken(',', &aToken.m_strParam2);

  // Suppress white spaces
  aToken.m_strParam1.Trim();
  aToken.m_strParam2.Trim();

  if( 2 == rc ) // token not found
  {
    aToken.m_TokenType = TemplateToken::LOOP_OVER;
    if( aToken.m_strParam1.empty() )
    {
      cerr << "Error: Missed variable name at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
      exit(1);
    }

    if( aToken.m_strParam2.empty() )
    {
      cerr << "Error: Missed NeXus path at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
      exit(1);
    }
    if( aToken.m_strParam2.StartWith('[') && aToken.m_strParam2.EndWith(']') )
      aToken.m_strParam2 = aToken.m_strParam2.substr(1, aToken.m_strParam2.size()-2 );
    else
    {
      cerr << "Error: Bad loop syntax at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
      exit(1);
    }
  }
  else
  {
    aToken.m_TokenType = TemplateToken::LOOP;
    // End value 
    aToken.m_strParam3 = *pstrLine;
    aToken.m_strParam3.Trim();
    if( aToken.m_strParam1.empty() )
    {
      cerr << "Error: Missed counter name at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
      exit(1);
    }

    if( aToken.m_strParam2.empty() )
    {
      cerr << "Error: Missed initial value for counter at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
      exit(1);
    }

    if( aToken.m_strParam3.empty() )
    {
      cerr << "Error: Missed final value for counter at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
      exit(1);
    }
  }


  m_vecToken.push_back(aToken);
  return m_vecToken.size()-1;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseIfCond
//-----------------------------------------------------------------------------
int TemplateFileParsor::ParseIfCond(String *pstrLine, int iLine)
{
  TemplateToken aToken;
  aToken.m_iTemplateLine = iLine;
  
  // Extract payload charge
  SuppressComments(pstrLine);
  pstrLine->Trim();

  String strCond, strParam2;
  uint uiPos = string::npos;
  if( string::npos == uiPos )
  {
    uiPos = pstrLine->rfind("!=");
    if( uiPos != string::npos )
    {
      aToken.m_TokenType = TemplateToken::IF_NOT_EQ;
      aToken.m_strParam1 = pstrLine->substr(0, uiPos);
      strParam2 = pstrLine->substr(uiPos + 2);
    }
  }
  if( string::npos == uiPos )
  {
    uiPos = pstrLine->rfind("=");
    if( uiPos != string::npos )
    {
      aToken.m_TokenType = TemplateToken::IF_EQ;
      aToken.m_strParam1 = pstrLine->substr(0, uiPos);
      strParam2 = pstrLine->substr(uiPos + 1);
    }
  }
  if( string::npos == uiPos )
  {
    uiPos = pstrLine->rfind(">");
    if( uiPos != string::npos )
    {
      aToken.m_TokenType = TemplateToken::IF_SUP;
      aToken.m_strParam1 = pstrLine->substr(0, uiPos);
      strParam2 = pstrLine->substr(uiPos + 1);
    }
  }
  if( string::npos == uiPos )
  {
    uiPos = pstrLine->rfind("<");
    if( uiPos != string::npos )
    {
      aToken.m_TokenType = TemplateToken::IF_INF;
      aToken.m_strParam1 = pstrLine->substr(0, uiPos);
      strParam2 = pstrLine->substr(uiPos + 1);
    }
  }

  aToken.m_strParam1.Trim();
  strParam2.Trim();

  // Remove quotes or double quotes if any
  strParam2.ExtractToken('\'', '\'', &aToken.m_strParam2);
  if( aToken.m_strParam2.empty() )
    strParam2.ExtractToken('"', '"', &aToken.m_strParam2);
  if( aToken.m_strParam2.empty() )
    // No quotes
    aToken.m_strParam2 = strParam2;

  m_vecToken.push_back(aToken);
  return m_vecToken.size()-1;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseIfExists
//-----------------------------------------------------------------------------
int TemplateFileParsor::ParseIfExists(String *pstrLine, int iLine)
{
  TemplateToken aToken;
  aToken.m_TokenType = TemplateToken::IF_EXISTS;
  aToken.m_iTemplateLine = iLine;

  // Extract payload charge
  SuppressComments(pstrLine);

  // we parse the name of the NeXus Object.
  pstrLine->Trim();
  if( pstrLine->StartWith("nxs:") )
    aToken.m_strData = *pstrLine;
  else // Variable
    aToken.m_strParam1 = *pstrLine;
  m_vecToken.push_back(aToken);
  return m_vecToken.size()-1;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseIf
//-----------------------------------------------------------------------------
int TemplateFileParsor::ParseIf(String *pstrLine, int iLine)
{
  // Search kind of 'if' statement
  pcsz pszLine = PSZ(*pstrLine);
  if( strstr(pszLine, "=") || strstr(pszLine, "!=") || strstr(pszLine, ">") || strstr(pszLine, "<") )
    return ParseIfCond(pstrLine, iLine);

  return ParseIfExists(pstrLine, iLine);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseBlockStart
//-----------------------------------------------------------------------------
int TemplateFileParsor::ParseBlockStart(String *pstrLine)
{
  // Extract payload charge
  SuppressComments(pstrLine);
  pstrLine->Trim();

  TemplateToken aToken;
  aToken.m_TokenType = TemplateToken::BLOCK_START;

  // Get block name
  pstrLine->ExtractToken(' ', &aToken.m_strParam1); 
  // Get max block length
  aToken.m_strParam2 = *pstrLine;

  m_vecToken.push_back(aToken);
  return m_vecToken.size()-1;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParsePadding
//-----------------------------------------------------------------------------
int TemplateFileParsor::ParsePadding(String *pstrLine, int iLine)
{
  // Extract payload charge
  SuppressComments(pstrLine);
  pstrLine->Trim();

  TemplateToken aToken;
  aToken.m_TokenType = TemplateToken::PADDING;

  // Get Padding pattern
  aToken.m_strParam1 = *pstrLine;
  if( pstrLine->empty() )
    // default padding pattern : 80 blanks characters and a LF
    aToken.m_strParam1 = "                                                                                \n";
  else
  {
    int iRc = pstrLine->ExtractToken('"', '"', &aToken.m_strParam1);
    if( iRc != String::SEP_FOUND )
    {
      cerr << "Error: bad syntax at line " << iLine << " in file " << m_strCurrentTemplateFile << "; quotes missing." << endl;
      exit(1);
    }
  }

  m_vecToken.push_back(aToken);
  return m_vecToken.size()-1;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::BuildDataFragments
//-----------------------------------------------------------------------------
bool TemplateFileParsor::BuildDataFragments(TemplateToken *pToken, const String &strData)
{
  String strToFragment = strData;
  bool bVarDetected = false;

  while( strToFragment.size() > 0 )
  {
    // Search for a variable
    uint uiFirstPos = strToFragment.find("$(");
    if( String::npos != uiFirstPos )
    {
      // Search for matching ')'. Take care of nested variables
      uint uiMatchPos = strToFragment.find_first_of(')', uiFirstPos + 2);

      if( String::npos != uiMatchPos )
      {
        // New fragment
        if( uiFirstPos > 0 )
        {
          DataFragment fragText(DataFragment::TEXT, strToFragment.substr(0, uiFirstPos));
          pToken->lstDataFragments.push_back(fragText);
        }

        // Delete up to '$(' characters
        strToFragment.erase(0, uiFirstPos + 2);

        // Extract variable content
        DataFragment fragVar(DataFragment::VARIABLE, strToFragment.substr(0, uiMatchPos - uiFirstPos - 2));
        pToken->lstDataFragments.push_back(fragVar);

        // Delete up to matching end parenthesis
        strToFragment.erase(0, uiMatchPos - uiFirstPos - 1);

        bVarDetected = true;
      }
      else
      {
        // Missing close bracket
        // Copying up to the end of template string
        DataFragment fragText(DataFragment::TEXT, strToFragment);
        pToken->lstDataFragments.push_back(fragText);
        strToFragment.erase();
      }
    }
    else
    {
      // Copying up to the end of template string
      DataFragment fragText(DataFragment::TEXT, strToFragment);
      pToken->lstDataFragments.push_back(fragText);
      strToFragment.erase();
    }
  }
  return bVarDetected;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::GetTypeFromFormat
//-----------------------------------------------------------------------------
DataBuf::Type TemplateFileParsor::GetTypeFromFormat(char cType, char cMod)
{
  switch( cType )
  {
    case 's':
      return DataBuf::CHAR;
    case 'd':
      if( cMod == 'l')
        return DataBuf::LONG;
      else if( cMod == 'h')
        return DataBuf::SHORT;
      return DataBuf::INT;
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
      return DataBuf::DOUBLE;
    case '?':
      return DataBuf::UNKNOWN;
    default:
      return DataBuf::UNKNOWN;
  };

  return DataBuf::UNKNOWN;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParsePrintData
//-----------------------------------------------------------------------------
void TemplateFileParsor::ParsePrintData(String *pstrLine, int iLine)
{
  if( pstrLine->StartWith("print", true) )
    pstrLine->erase(0, strlen("print"));
  pstrLine->Trim();

  String strToPrint;
  int iRc = pstrLine->ExtractToken('"', '"', &strToPrint);

  CTemplateProcessor VarProc(CTemplateProcessor::EMPTY_STRING);
  if( m_pVarEval )
    VarProc.AddEvaluator(m_pVarEval);
  CEnvVariableEvaluator EnvVar;
  VarProc.AddEvaluator(&EnvVar);

  // Variable substitution in quoted strings !!
  VarProc.Process(&strToPrint);

  uint uiPos = 0;
  // looking for new line characters.
  for( uiPos = strToPrint.find("\\n"); uiPos != String::npos; uiPos = strToPrint.find("\\n"))
    strToPrint.replace(uiPos, 2, "\n");

  // looking for carriage return characters.
  for( uiPos = strToPrint.find("\\r"); uiPos != String::npos; uiPos = strToPrint.find("\\r"))
    strToPrint.replace(uiPos, 2, "\r");

  // looking for tab characters.
  for( uiPos = strToPrint.find("\\t"); uiPos != String::npos; uiPos = strToPrint.find("\\t"))
    strToPrint.replace(uiPos, 2, "\t");

  if( 1 == iRc )
  {
    while( !strToPrint.empty() )
    {
      String strTxt, strTmp;
      // Search for next format directive
      bool bParse = true;
      while( bParse )
      {
        iRc = strToPrint.ExtractToken('%', &strTmp);
        // Skip directive indicator if it's a '%%' pair
        if( strToPrint.StartWith('%') )
        {
          strTmp += '%';
          strToPrint.erase(0, 1);
        }
        else
          bParse = false;
        strTxt += strTmp;
      }

      if( !strTxt.empty() )
      {
        // Text to print before format directive
        TemplateToken aToken;
        aToken.m_iTemplateLine = iLine;
        aToken.m_TokenType = TemplateToken::PRINT_TEXT;
        aToken.m_strData = strTxt;
        aToken.m_Format.Set("%s");
        m_vecToken.push_back(aToken);
      }

      if( 1 == iRc )
      {
        // A format directive was found
        SuppressComments(pstrLine);
        String strArgument;
        int iRc2 = pstrLine->ExtractToken(',', &strArgument);
        if( 0 == iRc2 )
        {
          cerr << "Error: Argument missing at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
          exit(1);
        }
        // Remove blanks
        strArgument.Trim();

        // Check argument type
        if( strArgument.StartWith("[") )
        {
          // This is a array type argument
          // Erase '[' and ']' enclosers
          strArgument.erase(0, 1); // '['
          uint uiMatchingPos = strArgument.find(']');
          if( uiMatchingPos == string::npos )
          {
            cerr << "Error: Missing ']' at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
            exit(1);
          }
          strArgument.erase(uiMatchingPos, 1); // ']'
 
          // Insert a loop/end-loop couple
          TemplateToken aLoopToken;
          aLoopToken.m_iTemplateLine = iLine;
          aLoopToken.m_TokenType = TemplateToken::LOOP_OVER;
          aLoopToken.m_strParam1 = "loopvar";

          // Extract loop part of the argument
          String strLastPart;
          int iLastPart = strArgument.rfind('>');
          strLastPart = strArgument.substr(iLastPart+1);
          aLoopToken.m_strParam2 = strArgument.substr(0, iLastPart+1);
          aLoopToken.m_iEndBlockPos = m_vecToken.size() + 1;
          // Add 'loop' Token
          m_vecToken.push_back(aLoopToken);

          // Replace loop element with variable access
          String strTmp;
          strArgument.ExtractTokenRight('<', '>', &strTmp);
          strArgument += "$(loopvar_name)" + strLastPart;
        }

        TemplateToken aToken;
        aToken.m_iTemplateLine = iLine;
        aToken.m_TokenType = TemplateToken::PRINT_DATA;

        if( false == BuildDataFragments(&aToken, strArgument) )
          // If no variable are presents in the argument string then fill strData member
          aToken.m_strData = strArgument;
        
        // Extend format string up to next format directive
        int iPos = strToPrint.find('%');
        // Parse format
        aToken.m_Format.Set('%' + strToPrint.substr(0, iPos));
        // Search for format type in first 10 characters
        DataBuf::Type eType = GetTypeFromFormat(aToken.m_Format.Type(), aToken.m_Format.Modifier());
        if( DataBuf::NO_TYPE == eType )
        {
          cerr << "Error: Bad type specification at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
          exit(1);
        }
        aToken.m_eOutputType = eType;
        aToken.m_strPrintFmt = aToken.m_Format.Get();

        m_vecToken.push_back(aToken);
        strToPrint.erase(0, iPos);
      }
    }
  }
  else
  {
    cerr << "Error: Missed '\"' at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
    exit(1);
  }
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseBinary
//-----------------------------------------------------------------------------
void TemplateFileParsor::ParseBinary(String *pstrLine, int iLine)
{
  if( pstrLine->StartWith("binary", true) )
    pstrLine->erase(0, strlen("binary"));
  pstrLine->Trim();

  String strBinaryFormat;
  int iRc = pstrLine->ExtractToken('\'', '\'', &strBinaryFormat);
  pstrLine->Trim();

  if( iRc != EXTRACT_TOKEN_FOUND )
  {
    cerr << "Error: Syntax error at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
    exit(1);
  }

  TemplateToken aToken;
  aToken.m_iTemplateLine = iLine;
  aToken.m_TokenType = TemplateToken::BINARY;

  if( false == BuildDataFragments(&aToken, *pstrLine) )
    // If no variable are presents in the argument string then fill strData member
    aToken.m_strData = *pstrLine;

  if( strBinaryFormat == "c" )
  {
    aToken.m_eBinaryDataType = TemplateToken::CHAR;
    aToken.m_uiTypeSize = 1;
  }
  else if( strBinaryFormat == "i1" || strBinaryFormat == "bt" )
  {
    aToken.m_eBinaryDataType = TemplateToken::BYTE;
    aToken.m_uiTypeSize = 1;
  }
  else if( strBinaryFormat == "i2" || strBinaryFormat == "si")
  {
    aToken.m_eBinaryDataType = TemplateToken::SHORT;
    aToken.m_uiTypeSize = sizeof(short);
  }
  else if( strBinaryFormat == "ui2" || strBinaryFormat == "usi")
  {
    aToken.m_eBinaryDataType = TemplateToken::USHORT;
    aToken.m_uiTypeSize = sizeof(unsigned short);
  }
  else if( strBinaryFormat == "i4" || strBinaryFormat == "li" )
  {
    aToken.m_eBinaryDataType = TemplateToken::LONG;
    aToken.m_uiTypeSize = sizeof(long);
  }
  else if( strBinaryFormat == "ui4" || strBinaryFormat == "uli" )
  {
    aToken.m_eBinaryDataType = TemplateToken::LONG;
    aToken.m_uiTypeSize = sizeof(unsigned long);
  }
  else if( strBinaryFormat == "f4" || strBinaryFormat == "f" )
  {
    aToken.m_eBinaryDataType = TemplateToken::FLOAT;
    aToken.m_uiTypeSize = sizeof(float);
  }
  else if( strBinaryFormat == "f8" || strBinaryFormat == "lf" )
  {
    aToken.m_eBinaryDataType = TemplateToken::DOUBLE;
    aToken.m_uiTypeSize = sizeof(double);
  }
  else if( strBinaryFormat == "b" )
  {
    aToken.m_eBinaryDataType = TemplateToken::RAW;
    aToken.m_uiTypeSize = 0;
  }
  else if( strBinaryFormat == "jpeg" )
  {
#ifdef __JPEG_SUPPORT__
    aToken.m_eBinaryDataType = TemplateToken::JPEG_IMAGE;
    aToken.m_uiTypeSize = 0;
#else
    cerr << "Error: jpeg output not supported in this version (line: " << iLine << " in file " << m_strCurrentTemplateFile << ")." << endl;
    exit(1);
#endif
  }
  else if( strBinaryFormat == "bmp" )
  {
    aToken.m_eBinaryDataType = TemplateToken::BMP_IMAGE;
    aToken.m_uiTypeSize = 0;
  }
  else
  {
    cerr << "Error: unknown binary format '" << strBinaryFormat << "' at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
    exit(1);
  }
  m_vecToken.push_back(aToken);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::Parse
//-----------------------------------------------------------------------------
bool TemplateFileParsor::IsLoopToken(const TemplateToken &aToken)
{
  if( TemplateToken::LOOP_OVER == aToken.m_TokenType ||
      TemplateToken::LOOP == aToken.m_TokenType )
    return true;
  return false;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::Parse
//-----------------------------------------------------------------------------
void TemplateFileParsor::Parse(const string &strTemplateName, IVariableEvaluator *pVarEval)
{
  m_pVarEval = pVarEval;
  m_fnMainTemplate.Set(strTemplateName);
  m_strCurrentTemplateFile = strTemplateName;
  Parse(strTemplateName);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::Parse
//-----------------------------------------------------------------------------
void TemplateFileParsor::Parse(const string &strTemplateName)
{
  m_strCurrentTemplateFile = strTemplateName;
  if( !FileExists(PSZ(m_strCurrentTemplateFile)) )
  {
    // Try with name relative to main template
    m_strCurrentTemplateFile = FileName(m_fnMainTemplate.Path() + m_strCurrentTemplateFile).FullName();
    if( !FileExists(PSZ(m_strCurrentTemplateFile)) )
    {
      cerr << "File " << strTemplateName << "don't exists" << endl;
      exit(1);
    }
  }

  // Build tokens tree
  ifstream fs(PSZ(m_strCurrentTemplateFile));

  int iLine = 0;
  // Tokenize
  Tokenize(fs, iLine);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ReadHeader
//-----------------------------------------------------------------------------
void TemplateFileParsor::ReadHeader(const string &strTemplateName, String *pstrParams)
{
  // Build template lines list
  ifstream fs(PSZ(strTemplateName));
  int iLine = 0;
  
  while( !fs.eof() )
  {
    iLine++;
    fs.getline(g_acScratchBuf, g_iScratchLen);
    String strLine = g_acScratchBuf;
    strLine.Trim();

    if( !strLine.StartWith("#") )
      // End of header reached
      return;

    // Header parsing
    // Search for lines begining with "# @param..."
    strLine.erase(0, 1);
    strLine.Trim();
    if( strLine.StartWith("@param") )
    {
      strLine.erase(0, strlen("@param"));
      strLine.Trim();

      // Get name
      String strName;
      strLine.ExtractToken(' ', &strName);
      strName.Trim();
      strLine.Trim();
      
      // Pass type;
      String strTmp;
      strLine.ExtractToken(' ', &strTmp);
      strLine.Trim();

      // Pass description
      strLine.ExtractToken('\'', '\'', &strTmp);
      strLine.Trim();
      *pstrParams += strName + string("=") + strLine + string(",");
    }
  }
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParseEndBlock
//-----------------------------------------------------------------------------
void TemplateFileParsor::ParseEndBlock(TemplateToken::Type eExpectedType, 
                                        TemplateToken::Type eTokenType,
                                        int iLine)
{
  if( eExpectedType != eTokenType )
  {
    cerr << "Error : end block mismatching at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
    exit(1);
  }
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::ParsePreprocessorInclude
//-----------------------------------------------------------------------------
void TemplateFileParsor::ParsePreprocessorInclude(String *pstrLine)
{
  if( pstrLine->StartWith("include", true) )
    pstrLine->erase(0, strlen("include"));
  if( pstrLine->StartWith("+", true) )
    pstrLine->erase(0, strlen("+"));
  pstrLine->Trim();

  String strFileName;
  pstrLine->ExtractToken('\'', '\'', &strFileName);
  Parse(strFileName);
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::GetLineToken
//-----------------------------------------------------------------------------
TemplateToken::Type TemplateFileParsor::GetLineToken(String &strLine)
{
  for( int i = 0; i < g_iTokenCount; i++ )
  {
    if( strLine.StartWith(g_LineToken[i].pszToken1) )
    {
      if( g_LineToken[i].bRemoveToken )
        strLine.erase(0, strlen(g_LineToken[i].pszToken1) );
      return g_LineToken[i].TokenType;
    }
  }
  return TemplateToken::NO_TYPE;
}

//-----------------------------------------------------------------------------
// TemplateFileParsor::Tokenize
//-----------------------------------------------------------------------------
void TemplateFileParsor::Tokenize(ifstream &fs, int &iLine, TemplateToken::Type eEndBlockToken)
{
  while( !fs.eof() )
  {
    iLine++;
    fs.getline(g_acScratchBuf, g_iScratchLen);
    String strLine = g_acScratchBuf;
    Trim(&strLine);
    // Merge splitted lines (with '\')
    while( EndWith(strLine, '\\') )
    {
      iLine++;
      strLine.erase(strLine.size()-1);
      fs.getline(g_acScratchBuf, g_iScratchLen);
      String strPartLine = g_acScratchBuf;
      Trim(&strPartLine);
      strLine += strPartLine;
    }

    // Replace tabs with spaces
    strLine.Replace('\t', ' ');

    if( strLine.size() == 0 || strLine.StartWith("//") || strLine.StartWith("#") )
      // Empty line
      continue;

    int iTokenType = (int)GetLineToken(strLine);
    switch( iTokenType )
    {
      case TemplateToken::PRINT:
        ParsePrintData(&strLine, iLine);
        break;
      case TemplateToken::LOOP:
      {
        int iPos = ParseLoop(&strLine, iLine);
        Tokenize(fs, iLine, TemplateToken::END_LOOP);
        // Store end block position in token vector
        m_vecToken[iPos].m_iEndBlockPos = m_vecToken.size() - 1;
        break;
      }
      case TemplateToken::IF:
      {
        int iPos = ParseIf(&strLine, iLine);
        Tokenize(fs, iLine, TemplateToken::END_IF);
        // Store end block position in token vector
        m_vecToken[iPos].m_iEndBlockPos = m_vecToken.size() - 1;
        break;
      }
      case TemplateToken::BLOCK_START:
        {
          int iPos = ParseBlockStart(&strLine);
          Tokenize(fs, iLine, TemplateToken::BLOCK_END);
          // Store end block position in token vector
          m_vecToken[iPos].m_iEndBlockPos = m_vecToken.size() - 1;
          break;
        }
      case TemplateToken::END_LOOP:
        ParseEndBlock(eEndBlockToken, TemplateToken::END_LOOP, iLine);
        return;
      case TemplateToken::END_IF:
        ParseEndBlock(eEndBlockToken, TemplateToken::END_IF, iLine);
        return;
      case TemplateToken::BLOCK_END:
        ParseEndBlock(eEndBlockToken, TemplateToken::BLOCK_END, iLine);
        return;
      case TemplateToken::SET:
        ParseSet(&strLine, iLine);
        break;
      case TemplateToken::OUTPUT:
        ParseOutput(&strLine, iLine, false);
        break;
      case TemplateToken::BINARY_OUTPUT:
        ParseOutput(&strLine, iLine, true);
        break;
      case TemplateToken::BINARY:
        ParseBinary(&strLine, iLine);
        break;
      case TemplateToken::PADDING:
        ParsePadding(&strLine, iLine);
        break;
      case TemplateToken::INCLUDE:
        ParsePreprocessorInclude(&strLine);
        break;
      default:
        cerr << "Error: Bad syntax at line " << iLine << " in file " << m_strCurrentTemplateFile << "." << endl;
        exit(1);
    }
  }

  if( eEndBlockToken != TemplateToken::NO_TYPE )
  {
    // End of file reached while parsing a 'loop' or 'if' block
    cerr << "Error : Unexpected end of template file (" << m_strCurrentTemplateFile << ")." << endl;
    exit(1);
  }
}

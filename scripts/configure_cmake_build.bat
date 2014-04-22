@echo off
set NEXUS_THIRD_PARTY=C:\development\nexus-third-party

set HDF5_ROOT64=C:\Program Files\HDF_Group\HDF5\1.8.10
set HDF4_ROOT64=C:\Program Files\HDF_Group\HDF\4.2.8
set MXML_ROOT64=c:\Program Files\mxml

set HDF5_ROOT32=C:\Program Files (x86)\HDF_Group\HDF5\1.8.10
set HDF4_ROOT32=C:\Program Files (x86)\HDF_Group\HDF\4.2.8
set MXML_ROOT32=c:\Program Files (x86)\mxml

cmake -DNATIVE:PATH="%NEXUS_THIRD_PARTY%" -P dir_syntax.cmake
set /P NEXUS_THIRD_PARTY_CMAKE= < dir_syntax.txt
del /q dir_syntax.txt

cmake -DNATIVE:PATH="%HDF5_ROOT64%" -P dir_syntax.cmake
set /P HDF5_ROOT64_CMAKE= < dir_syntax.txt
del /q dir_syntax.txt

cmake -DNATIVE:PATH="%HDF5_ROOT32%" -P dir_syntax.cmake
set /P HDF5_ROOT32_CMAKE= < dir_syntax.txt
del /q dir_syntax.txt

rmdir /s /q build_win64
mkdir build_win64
cd build_win64
set HDF5_ROOT=%HDF5_ROOT64%
set HDF4_ROOT=%HDF4_ROOT64%
set MXML_ROOT=%MXML_ROOT64%
REM -DBUILD_TESTING:BOOL=ON -DUSE_SHARED_LIBS:BOOL=ON
cmake -G "Visual Studio 10 Win64" ^
   -DZLIB_ROOT="%HDF5_ROOT64_CMAKE%" ^
   -DNEXUS_THIRD_PARTY="%NEXUS_THIRD_PARTY_CMAKE%" ^
   -DCMAKE_PREFIX_PATH="%NEXUS_THIRD_PARTY_CMAKE%/win64" ^
   ..    
cd ..

rmdir /s /q build_win32
mkdir build_win32
cd build_win32
set HDF5_ROOT=%HDF5_ROOT32%
set HDF4_ROOT=%HDF4_ROOT32%
set MXML_ROOT=%MXML_ROOT32%
REM -DBUILD_TESTING:BOOL=ON -DUSE_SHARED_LIBS:BOOL=ON
cmake -G "Visual Studio 10" ^
   -DZLIB_ROOT="%HDF5_ROOT32_CMAKE%" ^
   -DNEXUS_THIRD_PARTY="%NEXUS_THIRD_PARTY_CMAKE%" ^
   -DCMAKE_PREFIX_PATH="%NEXUS_THIRD_PARTY_CMAKE%/win32" ^
   ..       
cd ..

@echo Now enter either build_win32 or build_win64 and type
@echo
@echo     cmake  --build .  --config RelWihDebInfo  --clean-first
@echo     cpack  -C RelWihDebInfo  CPackConfig.cmake

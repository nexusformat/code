
/* CUSTOMIZED TYPEMAPS FOR THE C-PYTHON MAPPING OF NEXUS FUNCTIONS USING SWIG */
/* Author: Hartmut Gilde */
/* (c) 2004 FRM2 */



/*---------------------------------------------------*/
/* SECTION 1: Auxiliary Functions */
/*---------------------------------------------------*/


%{
/* helper function to free the handle */	
void destroyHandle(void* nxh) {
	NXhandle oldnxh = (NXhandle)nxh;
	NXclose(&oldnxh);
	return;
}

/* helper function to free the link */	
void destroyLink(void* ln) {
	NXlink* oldln  = (NXlink*)ln;
	free(oldln);
	return;
}

/* helper function to determine memory size for memory allocation */
int getTypeSize(int typecode){
  switch(typecode){
  case NX_FLOAT32:
  case NX_INT32:
  case NX_UINT32:
    return 4;
    break;
  case NX_FLOAT64:
    return 8;
    break;
  case NX_INT16:
  case NX_UINT16:
    return 2;
    break;
  default:
    return 1;
    break;
  }
}

/* checks if the type of the given python object is compatible with the specified Nexus data type*/
int checkTypes(PyObject* py_obj, int nx_type) {
	if (PyInt_Check(py_obj)) {
		if (  (nx_type == NX_INT8) ||
				(nx_type == NX_UINT8) ||
				(nx_type == NX_INT16) ||
				(nx_type == NX_UINT16) ||
				(nx_type == NX_INT32) ||
				(nx_type == NX_UINT32) ) {
			return 1;
		}
	}
	else if (PyLong_Check(py_obj)) {
		if (  (nx_type == NX_INT8) ||
				(nx_type == NX_UINT8) ||
				(nx_type == NX_INT16) ||
				(nx_type == NX_UINT16) ||
				(nx_type == NX_INT32) ||
				(nx_type == NX_UINT32) ) {
			return 1;
		}
	}
	else if (PyFloat_Check(py_obj)) {
		if (  (nx_type == NX_FLOAT32) ||
				(nx_type == NX_FLOAT64) ) {
			return 1;
		}
	}
	else if (PyString_Check(py_obj)) {
		if (  (nx_type == NX_CHAR)  ) {
			return 1;
		}
	}
	else {
		return 0;
	}
	return 0;
}

/* converts an arbitrary nested list of Python objects to a flat list */
int flattenList(PyObject* list, int rank, int dims[], int depth, int type, PyObject** target) {
	PyObject* litem;
	int i, lsize;

	if (PyList_Check(list)) {
		lsize = PyList_Size(list);
		/*if ((lsize != dims[depth]) || (rank == depth)) {
			printf("lsize %d, dims %d, rank %d, depth %d \n", lsize, dims[depth],rank, depth);	
			PyErr_SetString(PyExc_ValueError, "Dimensions don't match. Check your data\n");
			return 0;
		}*/
		for (i=0; i<lsize; i++) {
			litem = PyList_GetItem(list, i);
			if (!flattenList(litem, rank, dims, depth+1, type, target)) {
				return 0;
			}
		}	
	}
	else {
		if (checkTypes(list, type)) {
			if (PyLong_Check(list)) {
				PyObject* o = PyInt_FromLong(PyLong_AsLong(list));
				Py_XDECREF(list);
				list = o;
			}
			PyList_Append(*target, list);
		}
		else {
			PyErr_SetString(PyExc_StandardError, "Types don't match. Please Check your data\n");
			return 0;
		}
	}
	return 1;
}


/* converts the contents of a flat python list into c data */
void extractList(PyObject* flatlist, void** data, int type) {
	int i, lsize;
	void* tdim;
	PyObject* cur;
	
	lsize = PyList_Size(flatlist);
	
		switch(type) {
			case NX_INT8: 	/*	Data type for 1-byte signed integer */
				{	
					char *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (char)PyInt_AsLong(cur);
					}
				}
				break;
			case NX_UINT8: 	/*	Data type for 1-byte unsigned integer */
				{	
					unsigned char *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (unsigned char)PyInt_AsLong(cur);
					}
				}
				break;
			case NX_INT16: 	/*	Data type for 2-byte signed integer */
				{	
					short *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (short)PyInt_AsLong(cur);
					}
				}
				break;
			case NX_UINT16: 	/*	Data type for 2-byte unsigned integer */
				{	
					unsigned short *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (unsigned short)PyInt_AsLong(cur);
					}
				}
				break;
			case NX_INT32: 	/*	Data type for 4-byte signed integer */
				{	
					int *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (int)PyInt_AsLong(cur);
					}
				}
				break;
			case NX_UINT32: 	/*	Data type for 4-byte unsigned integer */
				{	
					unsigned int *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (unsigned int)PyInt_AsLong(cur);
					}
				}
				break;
			case NX_FLOAT32: 	/* Data type for 4-byte floating point */
				{	
					float *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (float)PyFloat_AsDouble(cur);
					}
				}
				break;
			case NX_FLOAT64: 	/* Data type for 8-byte floating point */
				{	
					double *it;
					it = *data;
					for (i=0; i<lsize; i++, it++) {
						cur = PyList_GetItem(flatlist, i);
						*it = (double)PyFloat_AsDouble(cur);
					}
				}
				break;
			default:
				break;
		} 
}


/* constructs a hirarchical python list structure out of the c data and the dimension information given*/ 
void buildList(PyObject** baselist, int depth, int rank, int dims[], int type, char** it) {
	int i,j;
	PyObject* list;
	PyObject** listp;
	
	for (i=0; i<dims[depth]; i++) { 
		if (depth<rank-1) {
			list = PyList_New(0);
			listp = &list;
			buildList(listp, depth+1, rank, dims, type, it);
			PyList_Append(*baselist, list);
			Py_XDECREF(list);
		}
		else {
			char* c_it;
			unsigned char* uc_it;
			short* s_it;
			unsigned short* us_it;
			int* i_it;
			unsigned int* ui_it;
			float* f_it;
			double* d_it;
			
			switch(type) {
				case NX_INT8:
					c_it = (char*)*it;
					PyList_Append(*baselist, PyInt_FromLong((long)(*c_it)));
					break;
				case NX_UINT8:
					uc_it = (unsigned char*)*it;
					PyList_Append(*baselist, PyInt_FromLong((long)(*uc_it)));
					break;
				case NX_INT16:
					s_it = (short*)*it;
					PyList_Append(*baselist, PyInt_FromLong((long)(*s_it)));
					break;
				case NX_UINT16:
					us_it = (unsigned short*)*it;
					PyList_Append(*baselist, PyInt_FromLong((long)(*us_it)));
					break;
				case NX_INT32:
					i_it = (int*)*it;
					PyList_Append(*baselist, PyInt_FromLong((long)(*i_it)));
					break;
				case NX_UINT32:
					ui_it = (unsigned int*)*it;
					PyList_Append(*baselist, PyInt_FromLong((long)(*ui_it)));
					break;
				case NX_FLOAT32:
					f_it = (float*)*it;
					PyList_Append(*baselist, PyFloat_FromDouble((double)(*f_it)));
					break;
				case NX_FLOAT64:
					d_it = (double*)*it;
					PyList_Append(*baselist, PyFloat_FromDouble(*d_it));
					break;
				default:
					PyErr_SetString(PyExc_TypeError, "Unknown type or type not supported yet.\n");
					return;
					break;
			}
			*it += getTypeSize(type);
		}
	}
}


%}




/*---------------------------------------------------*/
/* SECTION 2: Python Typemaps */
/*--------------------------------------------------*/


/*--------------------------------- NXopen -------------------------------*/
%typemap(python,in, numinputs=0) NXhandle* pHandle {
	NXhandle* handle = malloc(sizeof(NXhandle));
	$1 = handle;
}

%typemap(python,argout) NXhandle* pHandle {
	PyObject* obj;	
	obj = PyCObject_FromVoidPtr($1, NULL);
   $result = t_output_helper($result,obj);
}


/*--------------------------------- NXclose, NXflush -------------------------------*/
%typemap(python,in) NXhandle* ppHandle {
	NXhandle* hp = (NXhandle*)0;
	if (PyCObject_Check($input)) {
		hp = (NXhandle*)PyCObject_AsVoidPtr($input);		
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Wrong Object Type. Expected an NXhandle object.\n");
		return NULL;
	}
	$1 = hp;
}


/*--------------------------------- NXopendata, NXcompmakedata -------------------------------*/
%typemap(python,in) int dim[] {
	int i;
	int dim[NX_MAXRANK];
	PyObject * cur;

	if (PyList_Check($input)) {
		int size = PyList_Size($input);
		for (i=0; i<size; i++) {
			cur = PyList_GetItem($input,i);
			if (PyInt_Check(cur)){
				dim[i]= PyInt_AsLong(cur);	
			}
			else if (PyLong_Check(cur)){
				dim[i]= PyLong_AsLong(cur);
			}
			else {
				PyErr_SetString(PyExc_TypeError, "Wrong Type. Expected an integer.\n");
				return NULL;
			}
		}
	}
	$1 = dim;
}


/* ------------------------- NXcompmakedata ---------------------*/
/* Typemaps fot the NXcompmakedata function */
%typemap(python,in) int bufsize[] {
	PyObject * cur;
	
	int i, lsize, bufsize[NX_MAXRANK];

	if (PyList_Check($input)) {
		lsize = PyList_Size($input);
		for (i=0; i<lsize; i++) {
			cur = PyList_GetItem($input,i);
			if (PyInt_Check(cur)){
				bufsize[i]= PyInt_AsLong(cur);	
			}
			else if (PyLong_Check(cur)){
				bufsize[i]= PyLong_AsLong(cur);
			}
			else {
				PyErr_SetString(PyExc_TypeError, "Wrong Type. Expected an integer.\n");
				return NULL;
			}
		}
	}
	$1 = bufsize;
}




/* ------------------------- NXgetdata ---------------------*/
/*data_out acts as output argument*/
%typemap(python,in, numinputs=0) void* data_out {
	
}

%typemap(python,check) void* data_out {
	NXstatus t_result;
	
	void *data = (void*)0;
	
	int rank, type, dims[NX_MAXRANK];
	int *rankp = &rank;
	int *typep = &type;
	int *dimsp = dims;
	
   t_result = (NXstatus)NXgetinfo(arg1, rankp, dimsp, typep);
	
   if (t_result != NX_OK) {
		return NULL;
	}
	
	/* for NX_CHAR increase buffer to store '\0' character */
	if (type == NX_CHAR && rank==1) {
		dims[0]++;		
	}
	
	t_result =(NXstatus)NXmalloc(&data, rank, dimsp, type);	
	
	if (t_result != NX_OK) {
			return NULL;
	}	
	
	$1 = data;
}


%typemap(python,argout) void* data_out {
	PyObject* list;
	PyObject** stringobj;
	
	NXstatus t_result;
	
	int rank, type, dims[NX_MAXRANK];
	int *rankp = &rank;
	int *typep = &type;
	int *dimsp = dims;
	
   t_result = (NXstatus)NXgetinfo(arg1, rankp, dimsp, typep);
	
   if (t_result == NX_OK) {
		char * it= (char*)$1;
		
		if (type==NX_CHAR) {
			if (rank>1) {
				PyErr_SetString(PyExc_ValueError, "Type NX_CHAR is restricted to 1 dimension.\n");
				return NULL;
			}
			list = PyString_FromString(it);
		}
		else {
			list = PyList_New(0);
			buildList(&list, 0, rank, dimsp, type, &it);
			/* if dim=1 and only one element is returned, don't return it as list*/
			if (!PyList_Check(PyList_GetItem(list, 0))) {
				if (PyList_Size(list) == 1) {
					list = PyList_GetItem(list, 0);
				}
			}
		}
	}
	$result = t_output_helper($result, list);	
}


/*--------------------------------- NXgetslab -------------------------------*/
/*data_out_slab acts as output argument */
%typemap(python,in, numinputs=0) void* data_out_slab {
	
}

%typemap(python,check) void* data_out_slab {
	NXstatus t_result;
	
	void *data = (void*)0;
	
	int rank, type, dims[NX_MAXRANK];
	int *rankp = &rank;
	int *typep = &type;
	int *dimsp = dims;
	
   t_result = (NXstatus)NXgetinfo(arg1, rankp, dimsp, typep);
   if (t_result != NX_OK) {
		return NULL;
	}
	
	t_result =(NXstatus)NXmalloc(&data, rank, arg4, type);	
	if (t_result != NX_OK) {
		return NULL;
	}	
	
	$1 = data;
}


%typemap(python,argout) void* data_out_slab {
	PyObject* list;
	
	NXstatus t_result;
	
	int rank, type, dims[NX_MAXRANK];
	int *rankp = &rank;
	int *typep = &type;
	int *dimsp = dims;
	
   t_result = (NXstatus)NXgetinfo(arg1, rankp, dimsp, typep);
	
   if (t_result == NX_OK) {
		char * it= (char*)$1;
		
		if (type==NX_CHAR) {
			if (rank>1) {
				PyErr_SetString(PyExc_ValueError, "Type NX_CHAR is restricted to 1 dimension.\n");
				return NULL;
			}
			list = PyString_FromString(it);
		}
		else {
			list = PyList_New(0);
			buildList(&list, 0, rank, arg4, type, &it);
		}
	}
	
	$result = t_output_helper($result, list);	
}


/*--------------------------------- NXgetattr -------------------------------*/
/*iType argument acts as input and output argument*/
%typemap(python,in) int* iType {
	int inout;
	
	if (!PyInt_Check($input)) {
		PyErr_SetString(PyExc_TypeError, "Wrong type. Expected an integer.\n");
		return NULL;
	}
	inout = (int)PyInt_AsLong($input);
	
	$1 = &inout;
}


%typemap(python,argout) int* iType {
	PyObject* result_obj = PyInt_FromLong((long)(*($1)));
	$result = t_output_helper($result,result_obj);
	free(arg3);
}


/*--------------------------------- NXgetattr -------------------------------*/
/*data_out_att acts as output parameter*/
%typemap(python,in, numinputs=0) void* data_out_att {
}

%typemap(python,check) void* data_out_att {
	int* data_type = arg5;
	/*int* data_length = arg4;*/

	void* data = (void*)0;
	int buf_size=1;

	if (*data_type == NX_CHAR) {
		buf_size = 4096; 
	}
	else {
		buf_size = getTypeSize(*data_type);
	}    
	
	data = (void*)malloc(buf_size);		
	memset(data,0,buf_size);
	
	$1 = data;
}


%typemap(python,argout) void* data_out_att {
	int* data_type = arg5;
	PyObject* result_obj;
	void* data = $1;
	
	switch(*data_type) {
		case NX_CHAR:
			result_obj = PyString_FromString((char*)data);
			break;
		case NX_INT8:
			result_obj = PyInt_FromLong((long)(*((char*)data)));
			break;
		case NX_UINT8:
			result_obj = PyInt_FromLong((long)(*((unsigned char*)data)));
			break;
		case NX_INT16:
			result_obj = PyInt_FromLong((long)(*((short*)data)));
			break;
		case NX_UINT16:
			result_obj = PyInt_FromLong((long)(*((unsigned short*)data)));
			break;
		case NX_INT32:
			result_obj = PyInt_FromLong((long)(*((int*)data)));
			break;
		case NX_UINT32:
			result_obj = PyInt_FromLong((long)(*((unsigned int*)data)));
			break;
		case NX_FLOAT32:
			result_obj = PyFloat_FromDouble((double)(*((float*)data)));
			break;
		case NX_FLOAT64:
			result_obj = PyFloat_FromDouble((double)(*((double*)data)));
			break;
		default:
			PyErr_SetString(PyExc_TypeError, "Unknown type or type not supported yet.\n");
			return NULL;
			break;
	}
   $result = t_output_helper($result,result_obj);
}


/*--------------------------------- NXgetattr -------------------------------*/
%typemap(python,in, numinputs=0) int* iDataLen {
}

%typemap(python,check) int* iDataLen {
	int initial_size = 1;
	if (*arg5==NX_CHAR) {
		initial_size = 4096;
	}
	$1 = &initial_size;
}


/* ------------------------- NXputdata ---------------------*/
/*Typemaps for the NXputdata function*/
%typemap(python,in) void* data_in_put {
	PyObject* flatlist;
	
	NXstatus t_result;
	
	int i, ntotal=1;
	void* data;
	
	int rank, type, dims[NX_MAXRANK];
	int *rankp = &rank;
	int *typep = &type;
	int *dimsp = dims;
	
   t_result = (NXstatus)NXgetinfo(arg1, rankp, dimsp, typep);

	if (t_result == NX_OK) {
		for (i=0; i<rank; i++) {
			ntotal = ntotal*dimsp[i];
		}
		
		if (type == NX_CHAR) {
			if (PyString_Check($input)) {
				data = (char*)malloc(PyString_Size($input)+1);
				strcpy(data, PyString_AsString($input));
			}
			else {
				PyErr_SetString(PyExc_TypeError, "Types don't match. Only Strings allowed\n");
				return NULL;
			}
		}
		else {
			flatlist = PyList_New(0);
			if (flattenList($input, rank, dimsp, 0, type, &flatlist)) {
				data = malloc(getTypeSize(type)*ntotal);
				extractList(flatlist, &data, type);
			}
			Py_XDECREF(flatlist);
		}
	}
	$1 = data;
}


/* ------------------------- NXputattr ---------------------*/
/* data_in_att needs to be processed in order to determine length of attribute*/
%typemap(python,in) void* data_in_att {
	PyObject * cur;
	PyObject * data_obj = $input;
	int data_type = arg5;
	int data_length = arg4;
	void* data = (void*)0;

	if (data_type==NX_CHAR) {
		data = malloc((strlen(PyString_AsString(data_obj))+1));
	}
	else {
		data = malloc((getTypeSize(data_type)));
	}
	
	if (checkTypes(data_obj, data_type)) {
		if (PyLong_Check(data_obj)) {
			PyObject* o = PyInt_FromLong(PyLong_AsLong(data_obj));
			Py_XDECREF(data_obj);
			data_obj = o;
		}
		switch(data_type) {
			case NX_CHAR:
				{
					strcpy(data, PyString_AsString(data_obj));	
				}	
				break;
			case NX_INT8: 	/*	Data type for 1-byte signed integer */
				{	
					char *it = data;
					*it = (char)PyInt_AsLong(data_obj);
				}
				break;
			case NX_UINT8: 	/*	Data type for 1-byte unsigned integer */
				{	
					unsigned char *it = data;
					*it = (unsigned char)PyInt_AsLong(data_obj);
				}
				break;
			case NX_INT16: 	/*	Data type for 2-byte signed integer */
				{	
					short *it = data;
					*it = (short)PyInt_AsLong(data_obj);
				}
				break;
			case NX_UINT16: 	/*	Data type for 2-byte unsigned integer */
				{	
					unsigned short *it = data;
					*it = (unsigned short)PyInt_AsLong(data_obj);
				}
				break;
			case NX_INT32: 	/*	Data type for 4-byte signed integer */
				{	
					int *it = data;
					*it = (int)PyInt_AsLong(data_obj);
				}
				break;
			case NX_UINT32: 	/*	Data type for 4-byte unsigned integer */
				{	
					unsigned int *it = data;
					*it = (unsigned int)PyInt_AsLong(data_obj);
				}
				break;
			case NX_FLOAT32: 	/* Data type for 4-byte floating point */
				{	
					float *it = data;
					*it = (float)PyFloat_AsDouble(data_obj);
				}
				break;
			case NX_FLOAT64: 	/* Data type for 8-byte floating point */
				{	
					double *it = data;
					*it = (double)PyFloat_AsDouble(data_obj);
				}
				break;
			default:
				break;
		}
	}
	else {
		free(data);
		PyErr_SetString(PyExc_TypeError, "Data Types don't match. Check your data.");
		return 0;
	}
	
	$1 = data;
}


/*--------------------------------- NXputattr -------------------------------*/
%typemap(python,in, numinputs=0) int iDataLen {
}

%typemap(python,check) int iDataLen {
	if (arg5==NX_CHAR) {
		$1 = strlen(PyString_AsString(obj2));
	}
	else {
		$1 = 1;
	}
}


/*--------------------------------- NXgetslab, NXputslab -------------------------------*/
/* Typemaps for the NXgetslab and NXputslab functions */
%typemap(python,in) int SlabIn[] {
	int i;
	int dim[NX_MAXRANK];
	PyObject * cur;

	if (PyList_Check($input)) {
		int size = PyList_Size($input);
		for (i=0; i<size; i++) {
			cur = PyList_GetItem($input,i);
			if (PyInt_Check(cur)){
				dim[i]= PyInt_AsLong(cur);	
			}
			else if (PyLong_Check(cur)){
				dim[i]= PyLong_AsLong(cur);
			}
			else {
				PyErr_SetString(PyExc_TypeError, "Wrong Type. Expected an integer.\n");
				return NULL;
			}
		}
	}
	$1 = dim;
}




/*--------------------------------- NXgetinfo -------------------------------*/
%typemap(python,in, numinputs=0) int dimension[] {
	int dim[NX_MAXRANK];
	$1 = dim;
}

%typemap(python,argout) int dimension[] {
	PyObject* list;
	PyObject* litem;
	int i;
	
	list = PyList_New(0);
	for (i=0; i< *arg2; i++) {
		litem = PyInt_FromLong((long)$1[i]);
		PyList_Append(list, litem);
		Py_XDECREF(litem);
	}
   $result = t_output_helper($result, list);
}




/*--------------------------------- NXgetdataID, NXgetgroupID -------------------------------*/
%typemap(python,in, numinputs=0) NXlink* outLink {
	NXlink* link = (NXlink*)malloc(sizeof(NXlink));
	$1 = link;
}

%typemap(python,argout) NXlink* outLink {
	PyObject* obj;
	obj = PyCObject_FromVoidPtr($1, NULL);
	
   $result = t_output_helper($result,obj);
}


/*--------------------------------- NXmakelink -------------------------------*/
%typemap(python,in) NXlink* pLink {
	NXlink* lp = (NXlink*)0;
	if (PyCObject_Check($input)) {
		lp = (NXlink*)PyCObject_AsVoidPtr($input);		
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Wrong Object Type. Expected an NXlink object.\n");
		return NULL;
	}
	$1 = lp;
}




/*--------------------------------- all NX functions except NXopen -------------------------------*/
%typemap(python,in) NXhandle handle {
	NXhandle* hp = (NXhandle*)0;
	if (PyCObject_Check($input)) {
		hp = (NXhandle*)PyCObject_AsVoidPtr($input);
	}
	else {
		PyErr_SetString(PyExc_TypeError, "Wrong Object Type. Expected an NXhandle object.\n");
		return NULL;
	}
	$1 = *hp;
}



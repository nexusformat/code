#testing low level functionality 

import unittest
import sys
import os
import numpy

sys.path.append("../")

#load the testing module
import nxs.napi


class NAPILowLevelTestCase(unittest.TestCase):
    #static members
    _fname = "NAPI_LL_TEST.nxs"

    _shape1 = [20,30]
    _shape2 = [10,20,30]
    _dshape = [nxs.UNLIMITED,20,30]

    def setUp(self):
        self._file = nxs.napi.open(self._fname,"w")

        self._file.makegroup("scan_1","NXentry")
        
        pass

    def tearDown(self):
        self._file.close()

        try:
            os.remove(self._fname)
        except:
            pass

    def test_constants(self):
        self.assertEqual(nxs.napi.NOSTRIP,128)
        self.assertEqual(nxs.napi.UNLIMITED,1)
        self.assertEqual(nxs.napi.MAXRANK,32)
        self.assertEqual(nxs.napi.MAXNAMELEN,64)
        self.assertEqual(nxs.napi.MAXPATHLEN,1024)

        d = nxs.napi._compression_code
        dr = {"none":100,"lzw":200,"rle":300,"huffman":400}
        self.assertDictEqual(dr,d)

        d = nxs.napi._nxtype_code
        dr = {"char":4,"float32":5,"float64":6,
              "int8":20,"uint8":21,"int16":22,"uint16":23,
              "int32":24,"uint32":25,"int64":26,"uint64":27}
        self.assertDictEqual(dr,d)

        d = nxs.napi._nxopen_mode
        dr = {"r":1,"rw":2,"w":3,"w4":4,"w5":5,"wx":6}
        self.assertDictEqual(d,dr)

        l = nxs.nxapi.H4SKIP
        lr = ['CDF0.0','_HDF_CHK_TBL_','Attr0.0','RIG0.0','RI0.0',
                'RIATTR0.0N','RIATTR0.0C']
        self.assertListEqual(l,lr)
    
    def test_file_open(self): 
        self.assertRaises(nxs.napi.NeXusError,nxs.napi.open,"bla.nxs","r")
       
        #should produce no exception
        f = nxs.napi.open("bla.nxs","w")

        f = nxs.napi.open("bla.nxs","rw")

        f.close()
        os.remove("bla.nxs")

    def test_file_attributes(self):

        self._file.makegroup("/data","NXentry")
        self._file.opengroup("/data")
        self._file.closegroup()
    

    def test_file_groups(self):

        #should not work - group does not exist
        self.assertRaises(nxs.napi.NeXusError,self._file.opengroup,"data")
        self.assertRaises(nxs.napi.NeXusError,self._file.opengroup,"/data/test")

        #this should work
        self._file.makegroup("data","NXentry")

        #try to open the group in various ways
        self.assertRaises(nxs.napi.NeXusError,self._file.opengrouppath,"/data/test/dir")
        self.assertRaises(ValueError,self._file.opengroup,"data","NXinstrument")

        #the documentation and the code is not very consistent with 
        #its exceptions - NeXusError sometimes ValueError

        self._file.opengrouppath("/data")   #should work
        self._file.openpath("/data")  #works too

        self.assertRaises(nxs.napi.NeXusError,self._file.openpath,"/data/test/dir")


    def test_file_compdata(self):
        pass

    def test_file_simpledata(self):
        pass


    def __test_slab_data(self,typecode):
        self._file.opengroup("scan_1")
       
        #--------------write data to disk---------------------------
        def write_slab(np,d,offset,shape):
            for i in range(np):
                offset[0] = i*shape[0]
                d[...] = i
                self._file.putslab(d,offset,shape)
        
        #-------------read data from disk----------------------------
        def read_slab(np,d,offset,shape):
            for i in range(np):
                d[...] = i
                offset[0] = i*shape[0]
                o = numpy.squeeze(self._file.getslab(offset,shape))
                self.assertListEqual(d.flatten().tolist(),o.flatten().tolist())

        #--------------------IO per frame-----------------------------
        #create and open dataset 
        self._file.makedata("data",dtype=typecode,shape=self._dshape)
        self._file.opendata("data")

        #create data 
        d = numpy.ones(self._shape1,dtype=typecode)
        offset = [0,0,0]
        shape = [1]+self._shape1

        #write data
        write_slab(10,d,offset,shape)

        #read data back
        d[...] = 1
        read_slab(10,d,offset,shape)
    
        #close dataset
        self._file.closedata()
        
        #--------------------IO per frame with compression-----------------
        #create and open dataset 
        self._file.compmakedata("data3",dtype=typecode,shape=self._dshape,
                mode="lzw",chunks=[1]+self._shape1)
        self._file.opendata("data")

        #create data 
        d = numpy.ones(self._shape1,dtype=typecode)
        offset = [0,0,0]
        shape = [1]+self._shape1

        #write data
        write_slab(10,d,offset,shape)

        #read data back
        d[...] = 1
        read_slab(10,d,offset,shape)
    
        #close dataset
        self._file.closedata()
      
        #------------------volume IO----------------------------------
        #create new dataset
        self._file.makedata("data2",dtype=typecode,shape=self._dshape)
        self._file.opendata("data2")

        #create data 
        d = numpy.ones(self._shape2,dtype=typecode)
        offset = [0,0,0]
        shape = self._shape2

        #write data
        write_slab(10,d,offset,shape)

        #read data back
        d[...] = 1
        read_slab(10,d,offset,shape)

        #close the dataset
        self._file.closedata()

        self._file.closegroup()

    def test_file_slabdata_uint16(self):
        self.__test_slab_data("uint16")

    def test_file_slabdata_int16(self):
        self.__test_slab_data("int16")

    def test_file_slabdata_uint32(self):
        self.__test_slab_data("uint32")
        
    def test_file_slabdata_uint32(self):
        self.__test_slab_data("int32")

    def test_file_slabdata_float32(self):
        self.__test_slab_data("float32")

    def test_file_slabdata_float64(self):
        self.__test_slab_data("float64")



def suite():
    suite = unittest.TestSuite()
    suite.addTest(NAPILowLevelTest('test_constants'))

    return suite

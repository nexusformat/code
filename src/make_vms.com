$!
$! $Id: make_vms.com,v 1.7 2000/08/09 13:48:11 faa Exp $
$!
$! Build command file for VMS
$!
$! Freddie Akeroyd, STFC ISIS facility, GB
$!
$!
$ SET VERIFY
$ DEFINE HDF5_ROOT AXPLIB$DISK:[HDF5.]  ! where you installed HDF5, trailing "." is needed
$ copt := /prefix=all /float=ieee /include=([-.include],hdf5_root:[include]) /define=(IN_NEXUS_LIBRARY=1,HDF5,H5_USE_16_API)
$!
$ CC 'copt napi5.c
$ CC 'copt napi.c
$ CC 'copt napiu.c
$ CC 'copt nxdataset.c
$ CC 'copt nxstack.c
$ CC 'copt stptok.c
$ delete/noconfirm libnexus.olb;*
$ lib/create/object libnexus.olb napi5,napi,napiu,nxdataset,nxstack,stptok

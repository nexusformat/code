!
! $Id$
!
! *** Sample test program for the NeXus Fortran 90 interface
!
program NXtest

   use NXmodule

   integer :: NXrank, NXdims(NX_maxrank), NXtype, entry_status, attr_status
   integer(kind=NXi1), dimension(4) :: i1_array = (/1, 2, 3, 4/)
   integer(kind=NXi2), dimension(4) :: i2_array = (/1000, 2000, 3000, 4000/)
   integer(kind=NXi4), dimension(4) :: i4_array = (/1000000, 2000000, 3000000, 4000000/)
   real(kind=NXr4), dimension(4,5) :: r4_array
   real(kind=NXr8), dimension(4,5) :: r8_array
   integer(kind=NXi4), dimension(4) :: i4_buffer
   real(kind=NXr4), dimension(4) :: r4_buffer
   real(kind=NXr8), dimension(16) :: r8_buffer
   integer(kind=NXi4) :: i
   real(kind=NXr4) :: r
   character(len=20) :: ch_buffer
   character(len=NX_maxnamelen) :: name, class
   type(NXhandle) :: fileid

   r4_array = reshape ((/(i*1.0_NXr4,i=1,20)/),(/4,5/))
   r8_array = reshape ((/(i*1.0_NXr8,i=1,20)/),(/4,5/))
! *** create file
   if (NXopen("NXtest.nxs", NXACC_CREATE, fileid) /= NX_OK) stop
   if (NXmakegroup(fileid, "entry", "NXentry") /= NX_OK) stop
   if (NXopengroup(fileid, "entry", "NXentry") /= NX_OK) stop
      if (NXmakedata(fileid, "ch_data", NX_CHAR, 1, (/10/)) /= NX_OK) stop
      if (NXopendata(fileid, "ch_data") /= NX_OK) stop
         if (NXputdata(fileid, "NeXus data") /= NX_OK) stop
      if (NXclosedata(fileid) /= NX_OK) stop
      if (NXmakedata(fileid, "i1_data", NX_INT8, 1, (/4/)) /= NX_OK) stop
      if (NXopendata(fileid, "i1_data") /= NX_OK) stop
         if (NXputdata(fileid, i1_array) /= NX_OK) stop
      if (NXclosedata(fileid) /= NX_OK) stop
      if (NXmakedata(fileid, "i2_data", NX_INT16, 1, (/4/)) /= NX_OK) stop
      if (NXopendata(fileid, "i2_data") /= NX_OK) stop
         if (NXputdata(fileid, i2_array) /= NX_OK) stop
      if (NXclosedata(fileid) /= NX_OK) stop
      if (NXmakedata(fileid, "i4_data", NX_INT32, 1, (/4/)) /= NX_OK) stop
      if (NXopendata(fileid, "i4_data") /= NX_OK) stop
         if (NXputdata(fileid, i4_array) /= NX_OK) stop
      if (NXclosedata(fileid) /= NX_OK) stop
      if (NXmakedata(fileid, "r4_data", NX_FLOAT32, 2, (/4,5/)) /= NX_OK) stop
      if (NXopendata(fileid, "r4_data") /= NX_OK) stop
         if (NXputdata(fileid, reshape(r4_array,(/size(r4_array)/))) /= NX_OK) stop
      if (NXclosedata(fileid) /= NX_OK) stop
      if (NXmakedata(fileid, "r8_data", NX_FLOAT64, 2, (/4,5/)) /= NX_OK) stop
      if (NXopendata(fileid, "r8_data") /= NX_OK) stop
         r8_buffer(1:4) = reshape(r8_array(1:4,5:5),(/4/))
         if (NXputslab(fileid, r8_buffer, (/1,5/), (/4,1/)) /= NX_OK) stop
         r8_buffer = reshape(r8_array(1:4,1:4),(/16/))
         if (NXputslab(fileid, r8_buffer, (/1,1/), (/4,4/)) /= NX_OK) stop
         if (NXputattr(fileid, "ch_attribute", "NeXus") /= NX_OK) stop
         if (NXputattr(fileid, "i4_attribute", 42) /= NX_OK) stop
         if (NXputattr(fileid, "r4_attribute", 3.14159265) /= NX_OK) stop
      if (NXclosedata(fileid) /= NX_OK) stop
   if (NXclosegroup(fileid) /= NX_OK) stop
   if (NXclose(fileid) /= NX_OK) stop
! *** read data
   if (NXopen("NXtest.nxs", NXACC_READ, fileid) /= NX_OK) stop
   if (NXopengroup(fileid, "entry", "NXentry") /= NX_OK) stop
   do
      entry_status = NXgetnextentry(fileid, name, class, NXtype)
      if (entry_status == NX_ERROR) then
         stop
      else if (entry_status == NX_EOD) then
         exit
      else if (entry_status == NX_OK) then
         if (NXopendata(fileid, name) /= NX_OK) stop
            if (NXgetinfo(fileid, NXrank, NXdims, NXtype) /= NX_OK) stop
            print *, trim(name)//" : ", NXdatatype(NXtype)
            if (NXtype == NX_CHAR) then
               if (NXgetdata(fileid, ch_buffer) /= NX_OK) stop
               print *, "Values : ", ch_buffer
            else if (NXtype == NX_INT8 .or. NXtype == NX_INT16 .or. NXtype == NX_INT32) then
               if (NXgetdata(fileid, i4_buffer) /= NX_OK) stop
               print *, "Values : ", i4_buffer
            else if (NXtype == NX_FLOAT32 .or. NXtype == NX_FLOAT64) then
               if (NXgetslab(fileid, r4_buffer, (/1,1/), (/4,1/)) /= NX_OK) stop
               print *, "Values : ", r4_buffer
               if (NXgetslab(fileid, r4_buffer, (/1,2/), (/4,1/)) /= NX_OK) stop
               print *, "       : ", r4_buffer
               if (NXgetslab(fileid, r4_buffer, (/1,3/), (/4,1/)) /= NX_OK) stop
               print *, "       : ", r4_buffer
               if (NXgetslab(fileid, r4_buffer, (/1,4/), (/4,1/)) /= NX_OK) stop
               print *, "       : ", r4_buffer
               if (NXgetslab(fileid, r4_buffer, (/1,5/), (/4,1/)) /= NX_OK) stop
               print *, "       : ", r4_buffer
            end if
            do
               attr_status = NXgetnextattr(fileid, name, NXdims(1), NXtype) 
               if (attr_status == NX_ERROR) then
                  stop
               else if (attr_status == NX_EOD) then
                  exit
               else if (attr_status == NX_OK) then
                  if (NXtype == NX_CHAR) then
                     if (NXgetattr(fileid, name, ch_buffer) /= NX_OK) stop
                     print *, trim(name)//" : ", ch_buffer
                  else if (NXtype == NX_INT32) then
                     if (NXgetattr(fileid, name, i) /= NX_OK) stop
                     print *, trim(name)//" : ", i
                  else if (NXtype == NX_FLOAT32) then
                     if (NXgetattr(fileid, name, r) /= NX_OK) stop
                     print *, trim(name)//" : ", r
                  end if
               end if
            end do
         if (NXclosedata(fileid) /= NX_OK) stop
      end if
   end do
   if (NXclosegroup(fileid) /= NX_OK) stop
   if (NXclose(fileid) /= NX_OK) stop

end program NXtest

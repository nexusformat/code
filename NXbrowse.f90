!------------------------------------------------------------------------------
! NeXus - Neutron & X-ray Common Data Format
!  
! Fortran 90 Browser
!
!$Id$
!
! Copyright (C) 1999, Ray Osborn
!                     Materials Science Division, Argonne National Laboratory
!  
! See NAPI.C or <http://www.neutron.anl.gov/NeXus/> for details
!
! 99/7/11 - Initial Release
!------------------------------------------------------------------------------

program NXbrowse

   use NXUmodule
   character(len=80) :: file_name, input_text, path
   type(NXhandle) :: file_id
   character(len=NX_MAXNAMELEN), allocatable :: name(:), class(:)
   character(len=NX_MAXNAMELEN) :: group_name, group_class, data_name, attr_name
   integer :: status, n, i, NXrank, NXtype, NXdims(NX_MAXRANK)
   logical :: single_element
   character(len=255) :: char_value
   integer(kind=NXi4) :: int_value
   integer(kind=NXi4), pointer :: int_array(:), int_2Darray(:,:), int_3Darray(:,:,:)
   real(kind=NXr4) :: real_value
   real(kind=NXr4), pointer :: real_array(:), real_2Darray(:,:), real_3Darray(:,:,:)
   real(kind=NXr8) :: dble_value
   real(kind=NXr8), pointer :: dble_array(:), dble_2Darray(:,:), dble_3Darray(:,:,:)

   write (unit=*, fmt="(a)", advance="no") " Give name of NeXus file : "
   read *, file_name
!Open input file
   if (NXopen (trim(file_name), NXACC_READ, file_id) /= NX_OK) then
      call NXerror ("Can't open "//trim(file_name))
      stop
   end if
!Input commands until the EXIT command is given
   path = "NX"
   do
      write (unit=*, fmt="(a)", advance="no") " "//trim(path)//"> "
      read "(a)", input_text
      select case (command(input_text))
         !Command is to print a directory of the current group
         case ("DIR")
            if (NXgetgroupinfo (file_id, n) /= NX_OK) cycle
            allocate (name(n), class(n))
            status = NXgroupdir (file_id, n, name, class)
            if (status == NX_OK) then
               do i = 1,n
                  if (class(i)(1:2) == "NX") then
                     print *, "  NX Group : "//trim(name(i))//" ("//trim(class(i))//")"
                  else if (class(i)(1:3) == "SDS") then
                     print *, "  NX Data  : "//trim(name(i))
                  end if
               end do
            end if
            deallocate (name, class)
         !Command is to open the specified group
         case ("OPEN")
            input_text = adjustl(input_text(5:len_trim(input_text)))
            read (input_text(1:index(input_text," ")), fmt="(a)") group_name
            if (NXUfindgroup (file_id, group_name, group_class) == NX_EOD) then
               call NXerror (trim(group_name)//" does not exist")
               cycle
            end if
            if (NXopengroup (file_id, group_name, group_class) /= NX_OK) cycle
            !Add the group to the prompt string
            path = trim(path)//"/"//trim(group_name)
         !Command is to print the values of the data
         !1) if the array index is specified, a single element is output
         !2) if no index is given, the minimum and maximum values of the array are output
         case ("READ")
            input_text = adjustl(input_text(5:len_trim(input_text)))
            !Check for evidence that an individual element has been specified
            if (index(input_text,"(") == 0) then
               read (input_text(1:index(input_text," ")), fmt="(a)") data_name
               single_element = .false.
            else
               read (input_text(1:index(input_text,"(")-1), fmt="(a)") data_name
               single_element = .true.
            end if
            !Check the specified data item exists
            if (NXUfinddata (file_id, data_name) == NX_EOD) then
               call NXerror (trim(data_name)//" does not exist")
               cycle
            end if
            !Open the data and obtain its type and rank details
            if (NXopendata (file_id, data_name) /= NX_OK) cycle
            if (NXgetinfo (file_id, NXrank, NXdims, NXtype) /= NX_OK) cycle
            if (single_element) then
               input_text = input_text(index(input_text,"(")+1:index(input_text,")")-1)
               if (dimcount(input_text) /= NXrank) then
                  call NXerror ("Invalid array index")
                  cycle
               end if
               read (input_text, fmt=*) NXdims(1:NXrank)
            end if
            !Output data according to data type
            select case (NXtype)
               case (NX_CHAR)
                  if (NXUreaddata (file_id, data_name, char_value) /= NX_OK) cycle
                  print *, "  "//trim(data_name)//" ["//trim(NXdatatype(NXtype))//"] = "//trim(char_value)
                  char_value = " "
               case (NX_INT8,NX_UINT8,NX_INT16,NX_UINT16,NX_INT32,NX_UINT32)
                  !Output data according to rank
                  select case (NXrank)
                     case (1)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, int_array, data_start=NXdims, data_size=(/1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", int_array(1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, int_array) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(int_array), " to ", maxval(int_array)
                           end if
                        end if
                        deallocate (int_array, stat = status)
                     case (2)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, int_2Darray, data_start=NXdims, data_size=(/1,1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", int_2Darray(1,1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, int_2Darray) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(int_2Darray), " to ", maxval(int_2Darray)
                           end if
                        end if
                        deallocate (int_2Darray, stat = status)
                     case (3)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, int_3Darray, data_start=NXdims, data_size=(/1,1,1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", int_3Darray(1,1,1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, int_3Darray) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(int_3Darray), " to ", maxval(int_3Darray)
                           end if
                        end if
                        deallocate (int_3Darray, stat = status)
                  end select
               case (NX_FLOAT32)
                  !Output data according to rank
                  select case (NXrank)
                     case (1)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, real_array, data_start=NXdims, data_size=(/1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", real_array(1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, real_array) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(real_array), " to ", maxval(real_array)
                           end if
                        end if
                        deallocate (real_array, stat = status)
                     case (2)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, real_2Darray, data_start=NXdims, data_size=(/1,1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", real_2Darray(1,1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, real_2Darray) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(real_2Darray), " to ", maxval(real_2Darray)
                           end if
                        end if
                        deallocate (real_2Darray, stat = status)
                     case (3)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, real_3Darray, data_start=NXdims, data_size=(/1,1,1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", real_3Darray(1,1,1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, real_3Darray) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(real_3Darray), " to ", maxval(real_3Darray)
                           end if
                        end if
                        deallocate (real_3Darray, stat = status)
                     end select
               case (NX_FLOAT64)
                  !Output data according to rank
                  select case (NXrank)
                     case (1)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, dble_array, data_start=NXdims, data_size=(/1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", dble_array(1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, dble_array) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(dble_array), " to ", maxval(dble_array)
                           end if
                        end if
                        deallocate (dble_array, stat = status)
                     case (2)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, dble_2Darray, data_start=NXdims, data_size=(/1,1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", dble_2Darray(1,1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, dble_2Darray) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(dble_2Darray), " to ", maxval(dble_2Darray)
                           end if
                        end if
                        deallocate (dble_2Darray, stat = status)
                     case (3)
                        if (single_element) then
                           if (NXUreaddata (file_id, data_name, dble_3Darray, data_start=NXdims, data_size=(/1,1,1/)) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", dble_3Darray(1,1,1)
                           end if
                        else
                           if (NXUreaddata (file_id, data_name, dble_3Darray) == NX_OK) then
                              print *, "  "//trim(data_name)//trim(dimstring(NXrank,NXdims))//" ["//trim(NXdatatype(NXtype))&
                              //"] = ", minval(dble_3Darray), " to ", maxval(dble_3Darray)
                           end if
                        end if
                        deallocate (dble_3Darray, stat = status)
                  end select
            end select
            if (single_element) cycle
            !Check for attributes
            do
               status = NXgetnextattr (file_id, attr_name, n, NXtype)
               if (status /= NX_OK) exit
               !Output attribute information according to type
               select case (NXtype)
                  case (NX_CHAR)
                     if (NXgetattr (file_id, attr_name, char_value) /= NX_OK) cycle
                     print *, "    "//trim(attr_name)//" = "//trim(char_value)
                     char_value = " "
                  case (NX_INT8,NX_UINT8,NX_INT16,NX_UINT16,NX_INT32,NX_UINT32)
                     if (NXgetattr (file_id, attr_name, int_value) == NX_OK) then
                        print *, "    "//trim(attr_name)//" = ",int_value
                     end if
                  case (NX_FLOAT32)
                     if (NXgetattr (file_id, attr_name, real_value) == NX_OK) then
                        print *, "    "//trim(attr_name)//" = ",real_value
                     end if
                  case (NX_FLOAT64)
                     if (NXgetattr (file_id, attr_name, dble_value) == NX_OK) then
                        print *, "    "//trim(attr_name)//" = ",dble_value
                     end if
               end select
            end do
         !Command is to close the current group
         case ("CLOSE")
            if (NXclosegroup (file_id) /= NX_OK) cycle
            !Remove the group from the prompt string
            path = path(1:(index(path,"/",back=.true.)-1))
         !Command is to print help information
         case ("HELP")
            print *, "NXbrowse commands : DIR"
            print *, "                    OPEN <group_name>"
            print *, "                    READ <data_name>"
            print *, "                    CLOSE"
            print *, "                    HELP"
            print *, "                    EXIT"
         !Command is to exit the program
         case ("EXIT","QUIT")
            exit
      end select
   end do
   status = NXclose (file_id)

contains

   !Returns the first word of input text in upper case
   function command (input_text) result (output_text)

      character(len=*), intent(in) :: input_text
      character(len=80) :: output_text
      integer :: i, letter

      output_text = " "
      do i = 1,len_trim(adjustl(input_text))
         if (input_text(i:i) == " ") exit
         letter = ichar(input_text(i:i))
         if (letter >= 97 .and. letter <= 122) letter = letter - 32
         output_text(i:i) = char(letter)
      end do

   end function command

   !Outputs the specified dimensions as a formatted string
   function dimstring (rank, dims) result (string)

      integer, intent(in) :: rank, dims(:)
      character(len=50) :: string
      character(len=10) :: buffer
      integer :: i

      if (rank == 1 .and. dims(1) == 1) then
         string = " "
      else
         string = "("
         do i = 1,rank
            write (buffer, fmt="(i10)") dims(i)
            string = trim(string)//trim(adjustl(buffer))//","
         end do
         string(len_trim(string):len_trim(string)) = ")"
      end if

   end function dimstring

   !Outputs the number of dimensions specified in an input string
   function dimcount (string) result (rank)

      character(len=50), intent(in) :: string
      integer :: rank
      integer :: i
      logical :: number_found

      if (verify(trim(string)," 0123456789,") /= 0) then
         rank = 0
      else
         i = 1
         rank = 1
         number_found = .false.
         do
            if (verify(string(i:i),"0123456789") == 0) then
               number_found = .true.
            else if (number_found .and. string(i:i) == ",") then
               rank = rank + 1
               number_found = .false.
            else
               rank = 0
               exit
            end if
            i = i + 1
            if (i > len_trim(string)) exit
         end do
         if (.not. number_found) rank = 0
      end if

   end function dimcount

end program NXbrowse

C
C $Id$
C
C *** Sample test program for NeXus FORTRAN 77 interface
C
      INCLUDE 'napif.inc'
      INTEGER NXRANK,NXDIMS(NX_MAXRANK),NXTYPE
      INTEGER ENTRY_STATUS,ATTR_STATUS,LENGTH
      INTEGER SLAB_START(2), SLAB_SIZE(2)
      BYTE I1_ARRAY(4) /1, 2, 3, 4/
      INTEGER*2 I2_ARRAY(4)
      DATA I2_ARRAY /1000, 2000, 3000, 4000/
      INTEGER*4 I4_ARRAY(4) 
      DATA I4_ARRAY /1000000, 2000000, 3000000, 4000000/
      REAL*4 R4_ARRAY(4,5)
      DATA R4_ARRAY /1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,
     +  15.,16.,17.,18.,19.,20./
      REAL*8 R8_ARRAY(4,5)
      DATA R8_ARRAY/1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,
     +  15.,16.,17.,18.,19.,20./
      BYTE I1_BUFFER(4)
      INTEGER*2 I2_BUFFER(4)
      INTEGER*4 I4_BUFFER(4)
      REAL*4 R4_BUFFER(4)
      REAL*8 R8_BUFFER(16)
      INTEGER*4 I
      REAL*4 R
      CHARACTER*64 NAME, CLASS, CH_BUFFER
      INTEGER FILEID(NXHANDLESIZE)

      NXDIMS(1) = 2
      NXDIMS(2) = 2
      IF (NXOPEN('NXtest.nxs', NXACC_CREATE, FILEID) .NE. NX_OK) STOP
      IF (NXMAKEGROUP(FILEID, 'entry', 'NXentry') .NE. NX_OK) STOP
      IF (NXOPENGROUP(FILEID, 'entry', 'NXentry') .NE. NX_OK) STOP
      IF (NXMAKEDATA(FILEID, 'ch_data', NX_CHAR, 1, 10) .NE. NX_OK) STOP
      IF (NXOPENDATA(FILEID, 'ch_data') .NE. NX_OK) STOP
      IF (NXPUTCHARDATA(FILEID, 'NeXus data') .NE. NX_OK) STOP
      IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      IF (NXMAKEDATA(FILEID, 'i1_data', NX_INT8, 1, 4) .NE. NX_OK) STOP
      IF (NXOPENDATA(FILEID, 'i1_data') .NE. NX_OK) STOP
      IF (NXPUTDATA(FILEID, I1_ARRAY) .NE. NX_OK) STOP
      IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      IF (NXMAKEDATA(FILEID, 'i2_data', NX_INT16, 1, 4) .NE. NX_OK) STOP
      IF (NXOPENDATA(FILEID, 'i2_data') .NE. NX_OK) STOP
      IF (NXPUTDATA(FILEID, I2_ARRAY) .NE. NX_OK) STOP
      IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      IF (NXMAKEDATA(FILEID, 'i4_data', NX_INT32, 1, 4) .NE. NX_OK) STOP
      IF (NXOPENDATA(FILEID, 'i4_data') .NE. NX_OK) STOP
      IF (NXPUTDATA(FILEID, I4_ARRAY) .NE. NX_OK) STOP
      IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      NXDIMS(1) = 4
      NXDIMS(2) = 5
      IF (NXMAKEDATA(FILEID, 'r4_data', NX_FLOAT32, 2, NXDIMS) 
     +     .NE. NX_OK) STOP
      IF (NXOPENDATA(FILEID, 'r4_data') .NE. NX_OK) STOP
      IF (NXPUTDATA(FILEID, R4_ARRAY) .NE. NX_OK) STOP
      IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      IF (NXMAKEDATA(FILEID, 'r8_data', NX_FLOAT64, 2, NXDIMS) 
     +     .NE. NX_OK) STOP
      IF (NXOPENDATA(FILEID, 'r8_data') .NE. NX_OK) STOP
      SLAB_START(1) = 1
      SLAB_START(2) = 5
      SLAB_SIZE(1) = 4
      SLAB_SIZE(2) = 1
      IF (NXPUTSLAB(FILEID, R8_ARRAY(1,5), SLAB_START, SLAB_SIZE) 
     +     .NE. NX_OK) STOP
      SLAB_START(1) = 1
      SLAB_START(2) = 1
      SLAB_SIZE(1) = 4
      SLAB_SIZE(2) = 4
      IF (NXPUTSLAB(FILEID, R8_ARRAY, SLAB_START, SLAB_SIZE) 
     +     .NE. NX_OK) STOP
      IF (NXPUTCHARATTR(FILEID, 'ch_attribute', 'NeXus',5,NX_CHAR) 
     +     .NE. NX_OK) STOP
      IF (NXPUTATTR(FILEID, 'i4_attribute', 42, 1, NX_INT32) 
     +     .NE. NX_OK) STOP
      IF (NXPUTATTR(FILEID, 'r4_attribute', 3.14159265, 1, NX_FLOAT32) 
     +     .NE. NX_OK) STOP
      IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      IF (NXCLOSEGROUP(FILEID) .NE. NX_OK) STOP
      IF (NXCLOSE(FILEID) .NE. NX_OK) STOP
C *** read data
      IF (NXOPEN('NXtest.nxs', NXACC_READ, FILEID) .NE. NX_OK) STOP
      IF (NXOPENGROUP(FILEID, 'entry', 'NXentry') .NE. NX_OK) STOP
  100 ENTRY_STATUS=NXGETNEXTENTRY(FILEID,NAME,CLASS,NXTYPE)
      IF (ENTRY_STATUS .EQ. NX_ERROR) STOP
      IF (ENTRY_STATUS .EQ. NX_OK) THEN
         IF (NXOPENDATA(FILEID,NAME) .NE. NX_OK) STOP
         IF (NXGETINFO(FILEID,NXRANK,NXDIMS,NXTYPE) .NE. NX_OK) STOP
         WRITE(*,FMT='(1X,A,I2)') NAME(1:12)//' : ', NXTYPE
         IF (NXTYPE .EQ. NX_CHAR) THEN
            IF (NXGETCHARDATA(FILEID,CH_BUFFER) .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A)') 'Values       : '//CH_BUFFER
         ELSE IF (NXTYPE .EQ. NX_INT8) THEN
            IF (NXGETDATA(FILEID,I1_BUFFER) .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4I3)') 'Values       : ', I1_BUFFER
         ELSE IF (NXTYPE .EQ. NX_INT16) THEN
            IF (NXGETDATA(FILEID,I2_BUFFER) .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4I6)') 'Values       : ', I2_BUFFER
         ELSE IF (NXTYPE .EQ. NX_INT32) THEN
            IF (NXGETDATA(FILEID,I4_BUFFER) .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4I12)') 'Values       : ', I4_BUFFER
         ELSE IF (NXTYPE .EQ. NX_FLOAT32) THEN
            SLAB_START(1) = 1
            SLAB_START(2) = 1
            SLAB_SIZE(1) = 4
            SLAB_SIZE(2) = 1
            IF (NXGETSLAB(FILEID, R4_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') 'Values       : ', R4_BUFFER
            SLAB_START(2) = 2
            IF (NXGETSLAB(FILEID, R4_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ', R4_BUFFER
            SLAB_START(2) = 3
            IF (NXGETSLAB(FILEID, R4_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ', R4_BUFFER
            SLAB_START(2) = 4
            IF (NXGETSLAB(FILEID, R4_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ', R4_BUFFER
            SLAB_START(2) = 5
            IF (NXGETSLAB(FILEID, R4_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ', R4_BUFFER
         ELSE IF (NXTYPE .EQ. NX_FLOAT64) THEN
            SLAB_START(1) = 1
            SLAB_START(2) = 1
            SLAB_SIZE(1) = 4
            SLAB_SIZE(2) = 1
            IF (NXGETSLAB(FILEID, R8_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') 'Values       : ', 
     +        (R8_BUFFER(I), I=1,4)
            SLAB_START(2) = 2
            IF (NXGETSLAB(FILEID, R8_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ',  
     +        (R8_BUFFER(I), I=1,4)
            SLAB_START(2) = 3
            IF (NXGETSLAB(FILEID, R8_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ',  
     +        (R8_BUFFER(I), I=1,4)
            SLAB_START(2) = 4
            IF (NXGETSLAB(FILEID, R8_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ',  
     +        (R8_BUFFER(I), I=1,4)
            SLAB_START(2) = 5
            IF (NXGETSLAB(FILEID, R8_BUFFER, SLAB_START, SLAB_SIZE) 
     +            .NE. NX_OK) STOP
            WRITE(*,FMT='(1X,A,4F7.2)') '             : ', 
     +        (R8_BUFFER(I), I=1,4)
         END IF
  200    ATTR_STATUS = NXGETNEXTATTR (FILEID, NAME, NXDIMS, NXTYPE)
         IF (ATTR_STATUS .EQ. NX_ERROR) STOP
         IF (ATTR_STATUS .EQ. NX_OK) THEN
            IF (NXTYPE .EQ. NX_CHAR) THEN
               LENGTH=LEN(CH_BUFFER)
               IF (NXGETCHARATTR(FILEID,NAME,CH_BUFFER,LENGTH,NXTYPE) 
     +               .NE. NX_OK) STOP
               WRITE(*,FMT='(1X,A)') NAME(1:12)//' : '//CH_BUFFER
            ELSE IF (NXTYPE .EQ. NX_INT32) THEN
               LENGTH=1
               IF (NXGETATTR(FILEID,NAME,I,LENGTH,NXTYPE) 
     +               .NE. NX_OK) STOP
               WRITE(*,FMT='(1X,A,I5)') NAME(1:12)//' : ', I
            ELSE IF (NXTYPE .EQ. NX_FLOAT32) THEN
               LENGTH=1
               IF (NXGETATTR(FILEID,NAME,R,LENGTH,NXTYPE) 
     +               .NE. NX_OK) STOP
               WRITE(*,FMT='(1X,A,F10.6)') NAME(1:12)//' : ', R
            END IF
         END IF
         IF (ATTR_STATUS .NE. NX_EOD) GOTO 200
         IF (NXCLOSEDATA(FILEID) .NE. NX_OK) STOP
      END IF
      IF (ENTRY_STATUS .NE. NX_EOD) GOTO 100
      IF (NXCLOSEGROUP(FILEID) .NE. NX_OK) STOP
      IF (NXCLOSE(FILEID) .NE. NX_OK) STOP
      END

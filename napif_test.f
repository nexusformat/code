      INCLUDE 'napif.inc'
      INTEGER I,ARRAY_DIMS(2), RANK, DATATYPE, DIM_ARRAY(2)
      REAL ARRAY(4), DATA_ARRAY(4)
      CHARACTER*20 ATTR, BUFFER
      INTEGER FILEID(NXFILESIZE)
      ATTR = 'something'
      DO I=1,4
          ARRAY(I) = DBLE(I)
      ENDDO
      ARRAY_DIMS(1) = 2
      ARRAY_DIMS(2) = 2
      IF (NXOPEN('nxtest.dat', NXACC_CREATE, FILEID) .NE. NX_OK) STOP
      IF (NXMAKEGROUP(FILEID, 'mygroup', 'myclass') .NE. NX_OK) STOP
      IF (NXOPENGROUP(FILEID, 'mygroup', 'myclass') .NE. NX_OK) STOP
      IF (NXmakedata(fileid, 'mylabel', DFNT_FLOAT32, 2, array_dims) 
     +           .NE. NX_OK) STOP
      IF (NXopendata(fileid, 'mylabel') .NE. NX_OK) STOP
      IF (NXputdata(fileid, array) .NE. NX_OK) STOP
      IF (NXputattr(fileid, 'attribute', ATTR, LEN(ATTR)) 
     +           .NE. NX_OK) STOP
      IF (NXclosedata(fileid) .NE. NX_OK) STOP
      IF (NXclosegroup(fileid) .NE. NX_OK) STOP
      IF (NXclose(fileid) .NE. NX_OK) STOP
C *** read data
      IF (NXopen('nxtest.dat', NXACC_READ, fileid) .NE. NX_OK) STOP
      IF (NXopengroup(fileid, 'mygroup', 'myclass') .NE. NX_OK) STOP
      IF (NXopendata(fileid, 'mylabel') .NE. NX_OK) STOP
      IF (NXgetinfo(fileid, rank, dim_array, datatype) .NE. NX_OK) STOP
      IF (NXgetdata(fileid, data_array) .NE. NX_OK) STOP
      IF (NXgetattr(fileid, 'attribute', BUFFER, LEN(BUFFER)) 
     +           .NE. NX_OK) STOP
      DO I=1,4
          WRITE(6,*) DATA_ARRAY(I)
      ENDDO
      WRITE(6,*) 'Attribute: ',BUFFER
      IF (NXclosedata(fileid) .NE. NX_OK) STOP
      IF (NXclosegroup(fileid) .NE. NX_OK) STOP
      IF (NXclose(fileid) .NE. NX_OK) STOP
      END


IF( ${PACKAGE_NAME}_ENABLE_Experimental) 
  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    FECrsMatrix_UnitTests
    SOURCES
    FECrsMatrix_UnitTests
    ${TEUCHOS_STD_UNIT_TEST_MAIN}
    ARGS ""
    COMM serial mpi
    STANDARD_PASS_OUTPUT
    )
ENDIF()

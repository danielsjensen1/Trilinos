

TRIBITS_INCLUDE_DIRECTORIES(REQUIRED_DURING_INSTALLATION_TESTING ${CMAKE_CURRENT_SOURCE_DIR})

TRIBITS_INCLUDE_DIRECTORIES(REQUIRED_DURING_INSTALLATION_TESTING ${CMAKE_CURRENT_SOURCE_DIR}/../LOCA_TestProblems)

TRIBITS_INCLUDE_DIRECTORIES(REQUIRED_DURING_INSTALLATION_TESTING ${CMAKE_CURRENT_SOURCE_DIR}/../../utils)

IF(NOX_ENABLE_ABSTRACT_IMPLEMENTATION_EPETRA AND NOX_ENABLE_LOCA)

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    LOCA_HouseholderBorderedSolve
    SOURCES
    HouseholderBorderedSolve.C
    COMM serial mpi
    NUM_MPI_PROCS 2
    TESTONLYLIBS noxtestutils locaepetratestproblems
    ARGS -v
    PASS_REGULAR_EXPRESSION "All tests passed"
    )

 TRIBITS_ADD_EXECUTABLE_AND_TEST(
    LOCA_HouseholderTransposeBorderedSolve
    SOURCES
    HouseholderTransposeBorderedSolve.C
    COMM serial mpi
    NUM_MPI_PROCS 2
    TESTONLYLIBS noxtestutils locaepetratestproblems
    ARGS -v
    PASS_REGULAR_EXPRESSION "All tests passed"
    )

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    LOCA_TransposeSolve
    SOURCES
    TransposeSolve.C
    COMM serial mpi
    NUM_MPI_PROCS 2
    TESTONLYLIBS noxtestutils locaepetratestproblems
    ARGS -v
    PASS_REGULAR_EXPRESSION "All tests passed"
    )

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    LOCA_CompositeConstraint
    SOURCES
    CompositeConstraint.C
    COMM serial mpi
    NUM_MPI_PROCS 2
    TESTONLYLIBS noxtestutils locaepetratestproblems
    ARGS -v
    PASS_REGULAR_EXPRESSION "All tests passed"
    )

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    LOCA_CompositeConstraintMVDX
    SOURCES
    CompositeConstraintMVDX.C
    COMM serial mpi
    NUM_MPI_PROCS 2
    TESTONLYLIBS noxtestutils locaepetratestproblems
    ARGS -v
    PASS_REGULAR_EXPRESSION "All tests passed"
    )

  TRIBITS_ADD_EXECUTABLE_AND_TEST(
    LOCA_NaturalContResidualFills
    SOURCES
    NaturalContResidualFills.C
    COMM serial mpi
    NUM_MPI_PROCS 2
    TESTONLYLIBS noxtestutils locaepetratestproblems
    ARGS -v
    PASS_REGULAR_EXPRESSION "All tests passed"
    )

ENDIF()

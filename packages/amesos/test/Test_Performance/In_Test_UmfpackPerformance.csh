#!/bin/csh
# ************************************************************************
# 
#                 Amesos: Direct Sparse Solver Package
#                 Copyright (2004) Sandia Corporation
# 
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on behalf of the U.S. Government.
# 
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#  
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#  
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA
# Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
# 
# ************************************************************************

#
#  AmesosUmfpack.exe tests and times Umfpack on a MPI build.
#
#  RETURNS 0 if the test succeeds and 1 if the test fails 
#
#  Each call to amesos_test.exe performs one or more sovles on a particular 
#  matrix using a particular solver and prints a single line to SST.summary.
#  If the test worked, that line will contain OK.  Any line not containing 
#  the words OK, represents failure.
#  
#  Hence, the check for success is to make sure that all non-blank lines in 
#  SST.summary contain OK.
#
#  Each time that this script is run, it appends the result of any previous runs to 
#  AME.summary.  
#
#  More detailed logging information can be found in SST.log
#
#  A typical call to amesos_test.exe is:
#COMMENT       $mpigo 1 amesos_test.exe Amesos_Umfpack SuperLU.rua 0 1 1 0 1e-14 1e-14
#  where:
#     Amesos_Umfpack SuperLU.rua - The solver to use and the matrix to solve
#     0 1 1 0                 - MatrixType, Special, NumSolves, Transpose
#     1e-14 1e-14             - max residual, max error 
#
#
#   MatType = 0 means serial (all stored on process 0) 
#   MatType = 1 means distributed (evenly) 
#   Special = number of repeats (0 means run it once)
#   NumSolves < 0 means use multiple right hand sides
#   NumSolves > 1 means use blocked right hand sides
#
# Some machines use a command different than mpirun to run mpi jobs.  The
# test-harness.plx script sets the following environment variable
#  We test for this value below.  If not set, we set it to a default value.

set mpigo = `printenv TRILINOS_TEST_HARNESS_MPIGO_COMMAND`    # COMMENT 

if ("$mpigo" == "") then                                      # COMMENT
    set mpigo = "mpirun -np "                                 # COMMENT 
endif

touch SST.summary
cat >>AME.summary <SST.summary 
echo "COMMENT Start AmesosUmfpack.exe " > SST.summary 
echo "COMMENT The values printed in columns 11 and 12 are relative." >> SST.summary 
echo "COMMENT We test against absolute errors."   >> SST.summary 
echo "COMMENT column 1 - machine name " >> SST.summary 
echo "COMMENT column 2 - solver name " >> SST.summary 
echo "COMMENT column 3 - timestamp" >> SST.summary 
echo "COMMENT column 4 - matrix file " >> SST.summary 
echo "COMMENT column 5 - Matrix type  " >> SST.summary 
echo "COMMENT column 6 - Special - only used for SuperLU serial " >> SST.summary 
echo "COMMENT column 7 - Number of processes " >> SST.summary 
echo "COMMENT column 8 - Number of right hand sides, nrhs, (-1 means multiple solves) " >> SST.summary 
echo "COMMENT column 9 - Tranpose (1 == solve A^t x = b)" >> SST.summary 
echo "COMMENT column 10 - Norm of the matrix " >> SST.summary 
echo "COMMENT column 11 - relative error - i.e. error/norm(X) " >> SST.summary 
echo "COMMENT column 12 - residual error - i.e. residual/norm(B) " >> SST.summary 
echo "COMMENT column 13 - total_time " >> SST.summary 
echo "COMMENT column 14 - multiple solves only - Factor time " >> SST.summary 
echo "COMMENT column 15 - multiple solves only - Time for one solve " >> SST.summary 
echo "COMMENT column 16 - multiple solves only - Time for nrhs-1 solves " >> SST.summary 
echo "COMMENT column 17+ - summary " >> SST.summary 


#
#  Test one process, three processes and three processes transpose, tiny serial matrix, on Amesos_Umfpack
#
$mpigo 1 amesos_test.exe Amesos_Umfpack SuperLU.rua 0 1 1 0 1e-14 1e-14 >>SST.stdout
#  COMMENT fails on atlantis, sometimes janus    $mpigo 3 amesos_test.exe Amesos_Umfpack SuperLU.rua 0 1 1 0 1e-14 1e-14  >>SST.stdout
#  COMMENT fails on atlantis, sometimes janus    $mpigo 3 amesos_test.exe Amesos_Umfpack SuperLU.rua 0 1 1 1 1e-14 1e-14  >>SST.stdout
#
#  Test one process, three processes and three processes transposes, tiny distributed matrix, on Amesos_Umfpack
#
$mpigo 1 amesos_test.exe Amesos_Umfpack   fidapm05.rua 0 1 1 0 1000000000000000 1 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   fidapm05.rua 1 1 1 0 1000000000000000 1  >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   fidapm05.rua 1 1 1 1 1000000000000000 1  >>SST.stdout
#
#  Test some more small matrices
#
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 0 1 1 0 1e-10 1e-12 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 0 0 1 0 1e-10 1e-12  >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 0 1 1 1 1e-10 1e-12  >>SST.stdout
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolB.rua 0 1 1 0 1e-11 1e-13 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   ImpcolB.rua 0 1 1 0 1e-11 1e-14  >>SST.stdout
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolC.rua 0 1 1 0 1e-13 1e-13 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   ImpcolC.rua 0 0 1 0 1e-13 1e-13  >>SST.stdout
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolD.rua 0 1 1 0 1e-13 1e-13 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   ImpcolD.rua 0 1 1 0 1e-13 1e-13  >>SST.stdout
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolE.rua 0 1 1 0 1e-12 1e-11 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack   ImpcolE.rua 0 1 1 0 1e-12 1e-11  >>SST.stdout
#
#  Test mid sized matrices on 1 and 4 processes, half of them starting out serial, 
#  half starting out distributed.  (On the single process runs, distributed has no meaning.) 
#
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk24.rsa 0 1 1 0 1e-6  1e-1 >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk24.rsa 1 1 1 0 1e-6  1e-1 >>SST.stdout
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 1 1 1 0 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 0 1 1 0 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 1 0 1 0 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 0 0 1 0 1e-9 1e-4  >>SST.stdout

#
#  Test some tranpose solves
#
$mpigo 4 amesos_test.exe Amesos_Umfpack   ImpcolB.rua 0 1 1 1 1e-11 1e-12  >>SST.stdout
$mpigo 4 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 1 1 1 1 1e-10 1e-12  >>SST.stdout
$mpigo 4 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 1 1 3 1 1e-10 1e-12  >>SST.stdout
$mpigo 4 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 1 1 -3 1 1e-10 1e-12  >>SST.stdout


#
#  Test blocked right hand sides
#
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 0 1 2 0 1e-10 1e-12 >>SST.stdout
$mpigo 5 amesos_test.exe Amesos_Umfpack   ImpcolB.rua 0 1 4 0 1e-11 1e-14  >>SST.stdout
$mpigo 2 amesos_test.exe Amesos_Umfpack   ImpcolE.rua 0 1 6 0 1e-12 1e-11  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk24.rsa 0 1 3 0 1e-6  1e-1 >>SST.stdout
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 0 1 5 1 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 0 1 12 0 1e-9 1e-4  >>SST.stdout
#
#  Test multiple right hand sides
#
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolC.rua 0 1 -1 0 1e-13 1e-13 >>SST.stdout
$mpigo 5 amesos_test.exe Amesos_Umfpack   ImpcolD.rua 0 1 -2 1 1e-13 1e-13  >>SST.stdout
$mpigo 2 amesos_test.exe Amesos_Umfpack   ImpcolE.rua 0 1 -3 0 1e-12 1e-11  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk24.rsa 0 1 -4 1 1e-6  1e-1 >>SST.stdout
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 0 1 -5 0 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 0 1 -3 0 1e-9 1e-4  >>SST.stdout

#
#  Test blocked right hand sides with distributed matrix input
#
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolA.rua 1 1 2 0 1e-10 1e-12 >>SST.stdout
$mpigo 5 amesos_test.exe Amesos_Umfpack   ImpcolB.rua 1 1 4 0 1e-11 1e-14  >>SST.stdout
$mpigo 2 amesos_test.exe Amesos_Umfpack   ImpcolE.rua 1 1 6 0 1e-12 1e-11  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk24.rsa 1 1 3 0 1e-6  1e-1 >>SST.stdout
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 1 1 5 0 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 1 1 3 0 1e-9 1e-4  >>SST.stdout

#
#  Test multiple right hand sides with distributed matrix input
#
$mpigo 1 amesos_test.exe Amesos_Umfpack   ImpcolC.rua 1 1 -2 0 1e-13 1e-13 >>SST.stdout
$mpigo 5 amesos_test.exe Amesos_Umfpack   ImpcolD.rua 1 1 -3 0 1e-13 1e-13  >>SST.stdout
$mpigo 2 amesos_test.exe Amesos_Umfpack   ImpcolE.rua 1 1 -1 0 1e-12 1e-11  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk24.rsa 1 1 -2 0 1e-6  1e-1 >>SST.stdout
# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 1 1 -1 0 1e-9 1e-4  >>SST.stdout
# COMMENT $mpigo 4 amesos_test.exe Amesos_Umfpack   bcsstk18.rsa 1 1 -4 0 1e-9 1e-4  >>SST.stdout


#
#  Test some triplet files
#  The .triU files are unsymmatric, the .triS files are symmetric, providing 
#  either the upper or lower triangular part.
#
$mpigo 1 amesos_test.exe Amesos_Umfpack SuperLU.triU 0 1 1 0 1e-14 1e-14 >>SST.stdout
$mpigo 3 amesos_test.exe Amesos_Umfpack SuperLU.triU 0 1 1 0 1e-14 1e-14 >>SST.stdout

# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack K4989.triS 0 1 1 0 1e-10 1e-8 >>SST.stdout
# COMMENT $mpigo 2 amesos_test.exe Amesos_Umfpack K4989.triS 0 1 1 0 1e-10 1e-8 >>SST.stdout

# COMMENT $mpigo 1 amesos_test.exe Amesos_Umfpack K5000.triS 0 1 1 0 1e-10 1e-8 >>SST.stdout
# COMMENT $mpigo 6 amesos_test.exe Amesos_Umfpack K5000.triS 0 1 1 0 1e-10 1e-8 >>SST.stdout

# COMMENT $mpigo 6 amesos_test.exe Amesos_Umfpack K5000.triS 0 1 1 1 1e-10 1e-8 >>SST.stdout
# COMMENT $mpigo 6 amesos_test.exe Amesos_Umfpack K5000.triS 1 1 1 1 1e-10 1e-8 >>SST.stdout

$mpigo 1 amesos_test.exe Amesos_Umfpack Khead.triS 0 1 1 0 1e-13 1e-9 >>SST.stdout

echo "" >> SST.summary 
echo "COMMENT End AmesosUmfpack.exe" >> SST.summary 

#
#  Make sure that the tests ran 
#
set expected_lines = `grep mpigo AmesosUmfpack.csh | grep -v COMMENT | wc`
set results = `grep OK SST.summary | wc`
if ($results[1] != $expected_lines[1] ) then
    echo 'I expected ' $expected_lines[1] ' correct test results, but only saw: ' $results[1] 
    grep -v OK SST.summary | grep -v COMMENT | grep " " && echo "AmesosUmfpack Regression Test FAILED" 
    exit(1)
endif
#
#  Prints out success or failure and exit 
#
grep -v OK SST.summary | grep -v COMMENT | grep " " > /dev/null || echo "AmesosUmfpack Test passed on all" $expected_lines[1] " tests"
#
#  This should not generaly print anything as errors should have been caught in the if test above
#
grep -v OK SST.summary  | grep -v COMMENT | grep " " && echo "AmesosUmfpack Test FAILED" 
exit($status == 0)

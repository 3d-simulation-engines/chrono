//
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2010 Alessandro Tasora
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file at the top level of the distribution
// and at http://projectchrono.org/license-chrono.txt.
//


/// Class for interfacing with Pardiso Sparse Direct Solver 
/// from the Intel� MKL Library. 


#ifndef CHMKLENGINE_H
#define CHMKLENGINE_H

///////////////////////////////////////////////////
//
//   ChMklEngine.h
//
//   Use this header if you want to exploit Intel�
//	 MKL Library from Chrono::Engine programs.
//
//   HEADER file for CHRONO,
//  Multibody dynamics engine
//
// ------------------------------------------------
//             www.deltaknowledge.com
// ------------------------------------------------
///////////////////////////////////////////////////

#include "chrono_mkl/ChApiMkl.h"
#include <mkl.h>
#include "chrono_mkl/ChCSR3matrix.h"
#include "core/ChSpmatrix.h"



namespace chrono
{
	// REMEMBER: indeces start from zero; iparm[0] is "iparm(1)" in documentation
	// use IPARM instead to avoid misalignment due to different indexing; IPARM(1) == "iparm(1)" from documentation
#define IPARM(i) iparm[i-1]

	class ChApiMkl ChMklEngine {
	private:

		//ChCSR3Matrix system_matrix;

		void* pt[64]; //Handle to internal data structure (must be zeroed at startup)

		// Matrix in CSR3 format
		double* a;				// (pointer to the) array of non-zero elements of the A
		MKL_INT* ja;			// columns indices
		MKL_INT* ia;			// row index

		// rhs
		double* b;				// rhs

		// Output
		double* x;				// solution vector

		// Problem properties
		MKL_INT n;				//(square-)matrix size
		MKL_INT mtype;			// matrix type
		MKL_INT nrhs;			// number of rhs

		// Pardiso solver settings
		MKL_INT iparm[64];		// Pardiso solver parameter
		MKL_INT maxfct;			// maximum number of numerical factorizations
		MKL_INT* perm;			// permutation vector

		// Pardiso solver settings
		MKL_INT mnum;           // 1<=mnum<=maxfct : which factorizations to use; usually 1

		// Auxiliary variables
		int rhs_dimension;
		int sol_dimension;
		bool consistency_check; //< enables dimension consistency check
		int last_phase_called;

	public:

		ChMklEngine(int problem_size = 3, int matrix_type = 11);
		~ChMklEngine();

		/**Setting the linear system <tt>A*x=b</tt> to be solved means that the user must provide:
		*	- the matrix \c Z: in \c ChCSR3Matrix format or directly through the 3 CSR3 array format
		*	- the solution vector \c x: in any ChMatrix<> derived format or in bare C array
		*	- the unknowns vector \c b: in any ChMatrix<> derived format or in bare C array
		*/

		void SetMatrix(ChCSR3Matrix& Z);
		void SetMatrix(double* Z_values, int* Z_colIndex, int* Z_rowIndex);

		void SetSolutionVector(ChMatrix<>& insx);
		void SetSolutionVector(double* insx){ x = insx;  consistency_check = false; }

		void SetKnownVector(ChMatrix<>& insb);
		void SetKnownVector(ChMatrix<>& insf_chrono, ChMatrix<>& insb_chrono, ChMatrix<>& bdest);
		void SetKnownVector(double* insb){ b = insb; consistency_check = false; }

		void SetProblem(ChCSR3Matrix& Z, ChMatrix<>& insb, ChMatrix<>& insx);

		// Solver routine
		int PardisoCall(int set_phase = 13, int message_level = 0);
		void ResetSolver(int new_mat_type = 0); //< reinitializes the solver for the next iteration
		void SetProblemSize(int new_size) { n = new_size; }

		// Output functions
		void GetResidual(double* res) const;
		void GetResidual(ChMatrix<>& res) const { GetResidual(res.GetAddress()); }
		double GetResidualNorm(double* res) const;
		double GetResidualNorm(ChMatrix<>& res) const { return GetResidualNorm(res.GetAddress()); }

		// Auxiliary functions
		int* GetIparmAddress(){ return iparm; }
		void PrintIparmOutput();
		void SetConsistencyCheck(bool input){ consistency_check = input; }

	}; // ChMklEngine



} // end of namespace chrono


#endif
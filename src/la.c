/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*
 * Copyright (c) 2010-2011, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Neil T. Dantam <ntd@gatech.edu>
 * Georgia Tech Humanoid Robotics Lab
 * Under Direction of Prof. Mike Stilman <mstilman@cc.gatech.edu>
 *
 *
 * This file is provided under the following "BSD-style" License:
 *
 *
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 *
 */

// uncomment to check that local allocs actually get freed
// #define AA_ALLOC_STACK_MAX 0

#include "amino.h"

static int ilaenv( int ispec, const char *name, const char *opts,
                   int n1, int n2, int n3, int n4 ) {
    int nl = (int)strlen(name);
    int ol = (int)strlen(opts);
    return ilaenv_(&ispec, name, opts, &n1, &n2, &n3, &n4, nl, ol );
}

/*--- Scalar Ops ---*/


double aa_la_min( size_t n, const double *x ) {
    double v = HUGE_VAL;
    for( size_t i = 0; i < n; i++ )
        v = AA_MIN(v,x[i]);
    return v;
}

double aa_la_max( size_t n, const double *x ) {
    double v = -HUGE_VAL;
    for( size_t i = 0; i < n; i++ )
        v = AA_MAX(v,x[i]);
    return v;
}

double aa_la_dot( size_t n, const double *x, const double *y ) {
    double a = 0;
    for( size_t i = 0; i < n; i ++ )
        a += x[i]*y[i];
    return a;
}

double aa_la_norm( size_t n, const double *x ) {
    return sqrt( aa_la_dot( n, x, x ) );
}

double aa_la_ssd( size_t n, const double *x, const double *y ) {
    double a = 0;
    double t = 0;
    for( size_t i = 0; i < n; i ++ ) {
        t = x[i] - y[i];
        a += t*t;
    }
    return a;
}

double aa_la_dist( size_t n, const double *x, const double *y ) {
    return sqrt( aa_la_ssd(n,x,y) );
}

/*--- Vector Ops ---*/

void aa_la_sinc( size_t n, double alpha, double *x  ) {
    for( size_t i = 0; i < n; i ++ )
        x[i] += alpha;
}

void aa_la_vinc( size_t n, const double *x, double *y  ) {
    for( size_t i = 0; i < n; i ++ )
        y[i] += x[i];

}

void aa_la_axpy( size_t n, double alpha, const double *x, double *y  ) {
    for( size_t i = 0; i < n; i ++ )
        y[i] += alpha*x[i];
}

AA_API void aa_la_axpy3( size_t n, double alpha,
                         const double *x, const double *y, double *z ) {
    for( size_t i = 0; i < n; i ++ )
        z[i] = alpha*x[i] + y[i];
}

void aa_la_scal( size_t n, double alpha, double *x  ) {
    for( size_t i = 0; i < n; i ++ )
        x[i] *= alpha;
}

void aa_la_sadd( size_t n, double alpha, const double *x, double *r ) {
    for( size_t i = 0; i < n; i ++ )
        r[i] = alpha + x[i];
}

void aa_la_smul( size_t n, double alpha, const double *x, double *r ) {
    for( size_t i = 0; i < n; i ++ )
        r[i] = alpha * x[i];
}

void aa_la_ssub( size_t n, double alpha, const double *x, double *r ) {
    for( size_t i = 0; i < n; i ++ )
        r[i] = alpha - x[i];
}

void aa_la_sdiv( size_t n, double alpha, const double *x, double *r ) {
    for( size_t i = 0; i < n; i ++ )
        r[i] = alpha / x[i];
}

void aa_la_vadd( size_t n, const double *x, const double *y, double *r ) {
    for( size_t i = 0; i < n; i ++ ) {
        r[i] = x[i] + y[i];
    }
}

void aa_la_vsub( size_t n, const double *x, const double *y, double *r ){
    for( size_t i = 0; i < n; i ++ ) {
        r[i] = x[i] - y[i];
    }
}

void aa_la_vmul( size_t n, const double *x, const double *y, double *r ){
    for( size_t i = 0; i < n; i ++ ) {
        r[i] = x[i] * y[i];
    }
}

void aa_la_vdiv( size_t n, const double *x, const double *y, double *r ){
    for( size_t i = 0; i < n; i ++ ) {
        r[i] = x[i] / y[i];
    }
}

void aa_la_cross( const double a[3], const double b[3], double c[3] ) {
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}

void aa_la_normalize( size_t n, double *x ) {
    aa_la_scal( n, 1.0/aa_la_norm(n,x), x );
}

/*--- Matrix Ops --- */

AA_API void aa_la_transpose2( size_t m, size_t n, const double *A, double *At  ) {
    for( size_t i = 0; i < m; i ++ ) {
        for( size_t j = 0; j < n; j++ ) {
            AA_MATREF(At,n,j,i) = AA_MATREF(A,m,i,j);
        }
    }
}


AA_API void aa_la_transpose( size_t n, double *A  ) {
    for( size_t i = 0; i < n; i ++ ) {
        for( size_t j = i+1; j < n; j++ ) {
            double t = AA_MATREF(A,n,i,j);
            AA_MATREF(A,n,i,j) = AA_MATREF(A,n,j,i);
            AA_MATREF(A,n,j,i) = t;
        }
    }
}

double aa_la_trace( size_t n, const double *A ) {
    return cblas_dasum( (int)n, A, (int)(n+1) );
}

int aa_la_inv( size_t n, double *A ) {
    int ipiv[n];
    const int mi = (int) n;
    const int ni = (int) n;
    int info;

    // LU-factor
    dgetrf_( &mi, &ni, A, &mi, ipiv, &info );

    int lwork = -1;
    while(1) {
        double work[ lwork < 0 ? 1 : lwork ];
        dgetri_( &ni, A, &mi, ipiv, work, &lwork, &info );
        if( lwork > 0 ) break;
        assert( -1 == lwork && sizeof(work) == sizeof(double) );
        lwork = (int)work[0];
    }

    return info;
}

int aa_la_inv_( int *n, double *A ) {
    size_t ns = (size_t)*n;
    return aa_la_inv(ns,A);
}

void aa_la_dpinv( size_t m, size_t n, double k, const double *A, double *A_star ) {
    // A^T (AA^T + kI)^{-1}
    // A is m*n
    // x = Aq, x is m, q is n

    const int mi = (int)m;
    const int ni = (int)n;

    // this method uses an LU factorization
    /* // B = AA^T */
    /* double B[m*m]; */
    /* cblas_dgemm( CblasColMajor, CblasNoTrans, CblasTrans, mi, mi, ni, */
    /*              1, A, mi, A, mi, 0, B, mi ); */

    /* // B += kI */
    /* for( size_t i = 0; i < m; i ++ ) */
    /*    B[i*m+i] += k; */

    /* // B = B^-1 */
    /* aa_la_inv(m,B); */

    /* // A^* = A^T*B */
    /* cblas_dgemm( CblasColMajor, CblasTrans, CblasNoTrans, ni, mi, mi, */
    /*              1, A, mi, B, mi, 0, A_star, ni ); */

    // This method uses the SVD
    double U[m*m];
    double Vt[n*n];
    double S[AA_MIN(m,n)];

    // A = U S V^T
    aa_la_svd(m,n,A,U,S,Vt);

    memset( A_star, 0, sizeof(double)*m*n );
    // \sum s_i/(s_i**2+k) * v_i * u_i^T
    for( size_t i = 0; i < AA_MIN(m,n); i ++ ) {
        cblas_dger( CblasColMajor, ni, mi, S[i] / (S[i]*S[i] + k),
                Vt + i, ni,
                U + m*i, 1,
                A_star, ni
                );
    }
}

int aa_la_svd( size_t m, size_t n, const double *A, double *U, double *S, double *Vt ) {
    double Ap[m*n];
    aa_fcpy( Ap, A, m*n );

    const char *jobu = U ? "A" : "N";
    const char *jobvt = Vt ? "A" : "N";
    int mi = (int)m, ni=(int)n;
    int lwork = -1;
    int info;

    while(1) {
        double work[ lwork < 0 ? 1 : lwork ];
        dgesvd_( jobu, jobvt, &mi, &ni,
                 Ap, &mi,
                 S, U, &mi,
                 Vt, &ni,
                 &work[0], &lwork, &info );
        if( lwork >= 0 ) break;
        assert( -1 == lwork && sizeof(work) == sizeof(double) );
        lwork = (int) work[0];
    }

    //finish
    return info;
}

AA_API void aa_la_dls( size_t m, size_t n, double k, const double *A, const double *x, double *y ) {
    double A_star[m*n];
    aa_la_dpinv(m,n,k,A,A_star);
    aa_la_mvmul(n,m,A_star,x,y);
}

AA_API void aa_la_dlsnp( size_t m, size_t n, double k, const double *A, const double *x, const double *yp, double *y ) {
    double A_star[m*n];
    double B[n*n];

    aa_la_dpinv(m,n,k,A,A_star);
    aa_la_mvmul(n,m,A_star,x,y);

    // B = A^* A
    cblas_dgemm( CblasColMajor, CblasNoTrans, CblasNoTrans,
                 (int)n, (int)n, (int)m,
                 1, A_star, (int)n, A, (int)m, 0, B, (int)n );

    // B =  A^* A - I
    for( size_t i = 0; i < n; i ++ )
        AA_MATREF(B,n,i,i) -= 1;

    // y = y + -B yp
    cblas_dgemv( CblasColMajor, CblasNoTrans, (int)n, (int)n,
                 -1.0, B, (int)n,
                 yp, 1,
                 1, y, 1 );
}

static int dgelsd_smlsiz() {
    return ilaenv(9, "DGELSD", "", 0, 0, 0, 0 );
}

static int dgelsd_nlvl( int m, int n ) {
    return (int)AA_MAX(0, 1 + log2( AA_MIN(m,n) / (1 + dgelsd_smlsiz())));
}
static int dgelsd_miniwork(int m, int n) {
    int minmn = AA_MIN(m,n);
    return AA_MAX(1, 3 * minmn * dgelsd_nlvl(m,n) + 11 * minmn);
}

/* AA_API void aa_la_lls( size_t m, size_t n, size_t p, const double *A, const double *b, double *x ) { */
/*     int mi=(int)m, ni=(int)n, pi=(int)p; */
/*     double rcond=0; */
/*     int rank, info, lwork; */
/*     size_t liwork = (size_t)dgelsd_miniwork(mi,ni); */
/*     double *S = AA_NEW_LOCAL(double, AA_MIN(m,n)); */
/*     int *iwork = AA_NEW_LOCAL(int,liwork); */

/*     // calculate optimal work size */
/*     double qwork[1]; */
/*     lwork = -1; */
/*     dgelsd_( &mi, &ni, &pi,  */
/*             A, &mi, b, &mi,  */
/*             S, &rcond, &rank, */
/*             qwork, &lwork, iwork, &info ); */
/*     lwork=(int)qwork[0]; */

/*     /\*printf("info: %d, work: %f, iwork: %d\n", info, qwork[0],  dgelsd_miniwork(mi,ni));*\/ */
/*     // allocate work array */
/*     double *work = AA_NEW_LOCAL(double,(size_t)lwork); */

/*     // run */
/*     dgelsd_( &mi, &ni, &pi,  */
/*             A, &mi, b, &mi,  */
/*             S, &rcond, &rank, */
/*             work, &lwork, iwork, &info ); */

/*     // free */
/*     AA_DEL_LOCAL(S, double, AA_MIN(m,n)); */
/*     AA_DEL_LOCAL(work, double, (size_t)lwork); */
/*     AA_DEL_LOCAL(iwork, int, liwork); */

/* } */



AA_API void aa_la_linterp( size_t n,
                           double t0, const double *X0,
                           double t1, const double *X1,
                           double ti, double *Xi ) {
    double alpha =  (ti-t0)/(t1-t0);
    // x0 + alpha(x1-x0)
    // x0 - alpha*x0 + alpha*x1;
    // x0*(1-alpha) + x1*alpha
    for( size_t i = 0; i < n; i++ ) {
        Xi[i] = X0[i] + alpha*(X1[i]-X0[i]);
    }
}

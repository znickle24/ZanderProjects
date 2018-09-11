/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) {
  int i, j;

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) { naive_rotate(dim, src, dst); }

char rotate_first_try_descr[] = "rotate_first_try: blocking implementation";
void rotate_first_try(int dim, pixel *src, pixel *dst) {
  int ii, jj, i, j;
  for (ii = 0; ii < dim; ii+=8) {
    for (jj = 0; jj < dim; jj+=8) {
      for (i = ii; i < ii + 8; i++) {
        for (j = jj; j < jj + 8; j+=4) {
          dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
          dst[RIDX(dim-1-(j+1), i, dim)] = src[RIDX(i, (j+1), dim)];
          dst[RIDX(dim-1-(j+2), i, dim)] = src[RIDX(i, (j+2), dim)];
          dst[RIDX(dim-1-(j+3), i, dim)] = src[RIDX(i, (j+3), dim)];
        }
      }
    }
  }
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions() {
  add_rotate_function(&naive_rotate, naive_rotate_descr);
  add_rotate_function(&rotate, rotate_descr);
  add_rotate_function(&rotate_first_try, rotate_first_try_descr);
  /* ... Register additional test functions here */
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
  int red;
  int green;
  int blue;
  int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum) {
  sum->red = sum->green = sum->blue = 0;
  sum->num = 0;
  return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p) {
  sum->red += (int)p.red;
  sum->green += (int)p.green;
  sum->blue += (int)p.blue;
  sum->num++;
  return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) {
  current_pixel->red = (unsigned short)(sum.red / sum.num);
  current_pixel->green = (unsigned short)(sum.green / sum.num);
  current_pixel->blue = (unsigned short)(sum.blue / sum.num);
  return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src) {
  int ii, jj, sumnum;
  pixel_sum sum;
  pixel current_pixel;

  sum.red = sum.green = sum.blue = 0;
  sum.num = 0;
  for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++){
    for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++){
      pixel p = src[RIDX(ii, jj, dim)];
      sum.red += (int)p.red;
      sum.green += (int)p.green;
      sum.blue += (int)p.blue;
      sum.num++;
    }
    sumnum = sum.num;
    current_pixel.red = (unsigned short)(sum.red/sumnum);
    current_pixel.green = (unsigned short)(sum.green/sumnum);
    current_pixel.blue = (unsigned short)(sum.blue/sumnum);
  }
  return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) {
  int i, j;

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) { naive_smooth(dim, src, dst); }

/* smooth_first_try - using blocking and taking care of the edge cases to
make sure that the calculations aren't being done within each register_rotate_functions*/
char smooth_first_try_descr[] = "smooth_first_try: First Attempt";
void smooth_first_try(int dim, pixel *src, pixel *dst) {
  int i, j, a, b, block, edge, aMax, bMax;
  edge = dim-1;

  //corner cases -- need to re-do the avg
  dst[RIDX(0,0,dim)] = avg(dim, 0, 0, src);
  dst[RIDX(0, edge, dim)] = avg(dim, 0, edge, src);
  dst[RIDX(edge, 0, dim)] = avg(dim, edge, 0, src);
  dst[RIDX(edge, edge, dim)] = avg(dim, edge, edge, src);
  //edge cases -- need to edit the avg 
  for (i = 1; i < dim; i++) {
    dst[RIDX(i, 0, dim)] = avg(dim, i, 0, src);
    dst[RIDX(0, i, dim)] = avg(dim, 0, i, src);
  }
  for(i = edge; i > 0; i--) {
    dst[RIDX(i, edge, dim)] = avg(dim, i, edge, src);
    dst[RIDX(edge, i, dim)] = avg(dim, edge, i, src);
  }
  //central cases -- leave as initial average func
  block = 8;
  for (i = 1; i < edge; i += block) {
    for (j = 1; j < edge; j += block) {
      aMax = min(block, edge-i);
      bMax = min(block, edge-j);
      for (a = i; a < (aMax+i); a++) {
        for (b = j; b < (bMax+j); b++) {
          dst[RIDX(a, b, dim)] = avg(dim, a, b, src);
        }
      }
    }
  }
}
char smooth_second_try_descr[] = "smooth_second_try: second attempt blocking + unrolling";
void smooth_second_try (int dim, pixel *src, pixel*dst) {

  int i, j, a, b, block, edge, aMax, bMax;
  edge = dim-1;
  //corner cases
  dst[RIDX(0,0,dim)] = avg(dim, 0, 0, src);
  dst[RIDX(0, edge, dim)] = avg(dim, 0, edge, src);
  dst[RIDX(edge, 0, dim)] = avg(dim, edge, 0, src);
  dst[RIDX(edge, edge, dim)] = avg(dim, edge, edge, src);
  //edge cases
  for (i = 1; i < dim; i++) {
    dst[RIDX(i, 0, dim)] = avg(dim, i, 0, src);
    dst[RIDX(0, i, dim)] = avg(dim, 0, i, src);
  }
  for(i = edge; i > 0; i--) {
    dst[RIDX(i, edge, dim)] = avg(dim, i, edge, src);
    dst[RIDX(edge, i, dim)] = avg(dim, edge, i, src);
  }
  //central cases
  block = 8;
  for (i = 1; i < edge; i += block) {
    for (j = 1; j < edge; j += block) {
      aMax = min(block, edge-i);
      bMax = min(block, edge-j);
      for (a = i; a < (aMax+i); a++) {
        for (b = j; b < (bMax+j); b++) {
          dst[RIDX(a, b, dim)] = avg(dim, a, b, src);
        }
      }
    }
  }
}
/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
  add_smooth_function(&smooth, smooth_descr);
  add_smooth_function(&naive_smooth, naive_smooth_descr);
  add_smooth_function(&smooth_first_try, smooth_first_try_descr);
  add_smooth_function(&smooth_second_try, smooth_second_try_descr);
  /* ... Register additional test functions here */
}

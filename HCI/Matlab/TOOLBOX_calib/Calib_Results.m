% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 657.462413750577184 ; 657.946120844053212 ];

%-- Principal point:
cc = [ 303.137197777241511 ; 242.569961409558601 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.254025340318615 ; 0.121427132976134 ; -0.000208717485913 ; 0.000019582475285 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 0.318172587612137 ; 0.340445865987464 ];

%-- Principal point uncertainty:
cc_error = [ 0.646787861742838 ; 0.592154687068563 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.002482614410062 ; 0.009863174914791 ; 0.000133833853122 ; 0.000132167109237 ; 0.000000000000000 ];

%-- Image size:
nx = 640;
ny = 480;


%-- Various other variables (may be ignored if you do not use the Matlab Calibration Toolbox):
%-- Those variables are used to control which intrinsic parameters should be optimized

n_ima = 20;						% Number of calibration images
est_fc = [ 1 ; 1 ];					% Estimation indicator of the two focal variables
est_aspect_ratio = 1;				% Estimation indicator of the aspect ratio fc(2)/fc(1)
center_optim = 1;					% Estimation indicator of the principal point
est_alpha = 0;						% Estimation indicator of the skew coefficient
est_dist = [ 1 ; 1 ; 1 ; 1 ; 0 ];	% Estimation indicator of the distortion coefficients


%-- Extrinsic parameters:
%-- The rotation (omc_kk) and the translation (Tc_kk) vectors for every calibration image and their uncertainties

%-- Image #1:
omc_1 = [ 1.654928e+00 ; 1.651990e+00 ; -6.694039e-01 ];
Tc_1  = [ -1.777730e+02 ; -8.387799e+01 ; 8.531462e+02 ];
omc_error_1 = [ 7.555908e-04 ; 9.761484e-04 ; 1.248136e-03 ];
Tc_error_1  = [ 8.399214e-01 ; 7.748641e-01 ; 4.258424e-01 ];

%-- Image #2:
omc_2 = [ 1.849122e+00 ; 1.900688e+00 ; -3.966382e-01 ];
Tc_2  = [ -1.551112e+02 ; -1.594762e+02 ; 7.576528e+02 ];
omc_error_2 = [ 7.936737e-04 ; 9.698948e-04 ; 1.509021e-03 ];
Tc_error_2  = [ 7.498770e-01 ; 6.867583e-01 ; 4.185070e-01 ];

%-- Image #3:
omc_3 = [ 1.742596e+00 ; 2.077768e+00 ; -5.049744e-01 ];
Tc_3  = [ -1.253224e+02 ; -1.747127e+02 ; 7.755760e+02 ];
omc_error_3 = [ 7.264609e-04 ; 1.027708e-03 ; 1.560148e-03 ];
Tc_error_3  = [ 7.665749e-01 ; 7.028104e-01 ; 4.022871e-01 ];

%-- Image #4:
omc_4 = [ 1.828128e+00 ; 2.116859e+00 ; -1.103001e+00 ];
Tc_4  = [ -6.453703e+01 ; -1.549591e+02 ; 7.791371e+02 ];
omc_error_4 = [ 6.520720e-04 ; 1.064753e-03 ; 1.460823e-03 ];
Tc_error_4  = [ 7.725823e-01 ; 7.014389e-01 ; 3.240604e-01 ];

%-- Image #5:
omc_5 = [ 1.079365e+00 ; 1.922482e+00 ; -2.531012e-01 ];
Tc_5  = [ -9.239938e+01 ; -2.290583e+02 ; 7.368635e+02 ];
omc_error_5 = [ 6.368298e-04 ; 9.926789e-04 ; 1.118334e-03 ];
Tc_error_5  = [ 7.360089e-01 ; 6.694556e-01 ; 3.960755e-01 ];

%-- Image #6:
omc_6 = [ -1.701346e+00 ; -1.929109e+00 ; -7.917751e-01 ];
Tc_6  = [ -1.489513e+02 ; -7.970019e+01 ; 4.447630e+02 ];
omc_error_6 = [ 6.120162e-04 ; 9.908509e-04 ; 1.342029e-03 ];
Tc_error_6  = [ 4.407722e-01 ; 4.136243e-01 ; 3.386379e-01 ];

%-- Image #7:
omc_7 = [ 1.996583e+00 ; 1.931515e+00 ; 1.311023e+00 ];
Tc_7  = [ -8.303066e+01 ; -7.776482e+01 ; 4.401627e+02 ];
omc_error_7 = [ 1.171820e-03 ; 6.019909e-04 ; 1.408302e-03 ];
Tc_error_7  = [ 4.430396e-01 ; 4.043920e-01 ; 3.577563e-01 ];

%-- Image #8:
omc_8 = [ 1.961143e+00 ; 1.824220e+00 ; 1.326779e+00 ];
Tc_8  = [ -1.701874e+02 ; -1.035786e+02 ; 4.620009e+02 ];
omc_error_8 = [ 1.118699e-03 ; 6.142156e-04 ; 1.350864e-03 ];
Tc_error_8  = [ 4.842838e-01 ; 4.392796e-01 ; 4.029842e-01 ];

%-- Image #9:
omc_9 = [ -1.363619e+00 ; -1.980445e+00 ; 3.210451e-01 ];
Tc_9  = [ -1.954362e+00 ; -2.251611e+02 ; 7.284417e+02 ];
omc_error_9 = [ 7.626691e-04 ; 9.794004e-04 ; 1.262134e-03 ];
Tc_error_9  = [ 7.258108e-01 ; 6.589719e-01 ; 4.117976e-01 ];

%-- Image #10:
omc_10 = [ -1.513021e+00 ; -2.086648e+00 ; 1.888875e-01 ];
Tc_10  = [ -2.964605e+01 ; -3.004835e+02 ; 8.600782e+02 ];
omc_error_10 = [ 9.303164e-04 ; 1.113451e-03 ; 1.677677e-03 ];
Tc_error_10  = [ 8.724003e-01 ; 7.834523e-01 ; 5.465842e-01 ];

%-- Image #11:
omc_11 = [ -1.793031e+00 ; -2.064967e+00 ; -4.800587e-01 ];
Tc_11  = [ -1.511247e+02 ; -2.354832e+02 ; 7.046441e+02 ];
omc_error_11 = [ 8.348462e-04 ; 1.051025e-03 ; 1.806749e-03 ];
Tc_error_11  = [ 7.151754e-01 ; 6.709526e-01 ; 5.408340e-01 ];

%-- Image #12:
omc_12 = [ -1.838801e+00 ; -2.087240e+00 ; -5.158081e-01 ];
Tc_12  = [ -1.335500e+02 ; -1.773144e+02 ; 6.048330e+02 ];
omc_error_12 = [ 7.113465e-04 ; 1.009563e-03 ; 1.665790e-03 ];
Tc_error_12  = [ 6.090939e-01 ; 5.672985e-01 ; 4.520870e-01 ];

%-- Image #13:
omc_13 = [ -1.918715e+00 ; -2.116506e+00 ; -5.945314e-01 ];
Tc_13  = [ -1.327538e+02 ; -1.436472e+02 ; 5.446936e+02 ];
omc_error_13 = [ 6.635279e-04 ; 9.992346e-04 ; 1.637680e-03 ];
Tc_error_13  = [ 5.469621e-01 ; 5.078211e-01 ; 4.103108e-01 ];

%-- Image #14:
omc_14 = [ -1.954108e+00 ; -2.124529e+00 ; -5.849565e-01 ];
Tc_14  = [ -1.236535e+02 ; -1.372119e+02 ; 4.907432e+02 ];
omc_error_14 = [ 6.244782e-04 ; 9.794166e-04 ; 1.603271e-03 ];
Tc_error_14  = [ 4.934998e-01 ; 4.570690e-01 ; 3.682092e-01 ];

%-- Image #15:
omc_15 = [ -2.110566e+00 ; -2.253735e+00 ; -4.956820e-01 ];
Tc_15  = [ -1.992015e+02 ; -1.345716e+02 ; 4.748866e+02 ];
omc_error_15 = [ 7.211665e-04 ; 9.179003e-04 ; 1.748369e-03 ];
Tc_error_15  = [ 4.839197e-01 ; 4.533208e-01 ; 3.970393e-01 ];

%-- Image #16:
omc_16 = [ 1.887030e+00 ; 2.336243e+00 ; -1.738221e-01 ];
Tc_16  = [ -1.601007e+01 ; -1.704005e+02 ; 6.956748e+02 ];
omc_error_16 = [ 9.918786e-04 ; 1.047928e-03 ; 2.180040e-03 ];
Tc_error_16  = [ 6.888025e-01 ; 6.255925e-01 ; 4.702592e-01 ];

%-- Image #17:
omc_17 = [ -1.612582e+00 ; -1.953294e+00 ; -3.477439e-01 ];
Tc_17  = [ -1.352759e+02 ; -1.389987e+02 ; 4.899665e+02 ];
omc_error_17 = [ 6.169373e-04 ; 9.435409e-04 ; 1.330041e-03 ];
Tc_error_17  = [ 4.870724e-01 ; 4.533299e-01 ; 3.262920e-01 ];

%-- Image #18:
omc_18 = [ -1.341591e+00 ; -1.692586e+00 ; -2.972901e-01 ];
Tc_18  = [ -1.854396e+02 ; -1.578339e+02 ; 4.410354e+02 ];
omc_error_18 = [ 7.083066e-04 ; 9.165073e-04 ; 1.050289e-03 ];
Tc_error_18  = [ 4.427784e-01 ; 4.131883e-01 ; 3.170955e-01 ];

%-- Image #19:
omc_19 = [ -1.925664e+00 ; -1.837984e+00 ; -1.440615e+00 ];
Tc_19  = [ -1.066233e+02 ; -7.960101e+01 ; 3.340894e+02 ];
omc_error_19 = [ 6.093463e-04 ; 1.074275e-03 ; 1.361119e-03 ];
Tc_error_19  = [ 3.437198e-01 ; 3.154226e-01 ; 2.973730e-01 ];

%-- Image #20:
omc_20 = [ 1.895969e+00 ; 1.593180e+00 ; 1.471969e+00 ];
Tc_20  = [ -1.439594e+02 ; -8.800908e+01 ; 3.963760e+02 ];
omc_error_20 = [ 1.134669e-03 ; 6.279712e-04 ; 1.223544e-03 ];
Tc_error_20  = [ 4.197693e-01 ; 3.762027e-01 ; 3.599620e-01 ];


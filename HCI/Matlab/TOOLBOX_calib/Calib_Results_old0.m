% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 657.643745972416582 ; 658.041125667414121 ];

%-- Principal point:
cc = [ 303.192392427076243 ; 242.555662683688581 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.256098154011708 ; 0.130887284698685 ; -0.000191177709848 ; 0.000038483012955 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 0.402427447814932 ; 0.430563600768575 ];

%-- Principal point uncertainty:
cc_error = [ 0.818597846564539 ; 0.748803043091248 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.003142065621348 ; 0.012508238186850 ; 0.000169370043125 ; 0.000167546987090 ; 0.000000000000000 ];

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
omc_1 = [ 1.654720e+00 ; 1.651630e+00 ; -6.697971e-01 ];
Tc_1  = [ -1.778479e+02 ; -8.365803e+01 ; 8.532221e+02 ];
omc_error_1 = [ 9.548558e-04 ; 1.234566e-03 ; 1.577696e-03 ];
Tc_error_1  = [ 1.062967e+00 ; 9.798267e-01 ; 5.385127e-01 ];

%-- Image #2:
omc_2 = [ 1.848877e+00 ; 1.900269e+00 ; -3.969660e-01 ];
Tc_2  = [ -1.552066e+02 ; -1.592776e+02 ; 7.578523e+02 ];
omc_error_2 = [ 1.003394e-03 ; 1.226901e-03 ; 1.907814e-03 ];
Tc_error_2  = [ 9.491793e-01 ; 8.686433e-01 ; 5.292979e-01 ];

%-- Image #3:
omc_3 = [ 1.742277e+00 ; 2.077254e+00 ; -5.051086e-01 ];
Tc_3  = [ -1.254892e+02 ; -1.745481e+02 ; 7.757372e+02 ];
omc_error_3 = [ 9.182525e-04 ; 1.299980e-03 ; 1.972188e-03 ];
Tc_error_3  = [ 9.702793e-01 ; 8.889100e-01 ; 5.087796e-01 ];

%-- Image #4:
omc_4 = [ 1.827844e+00 ; 2.116467e+00 ; -1.102882e+00 ];
Tc_4  = [ -6.468451e+01 ; -1.547919e+02 ; 7.793400e+02 ];
omc_error_4 = [ 8.241556e-04 ; 1.346623e-03 ; 1.846831e-03 ];
Tc_error_4  = [ 9.778171e-01 ; 8.872203e-01 ; 4.098780e-01 ];

%-- Image #5:
omc_5 = [ 1.078974e+00 ; 1.922185e+00 ; -2.527138e-01 ];
Tc_5  = [ -9.248733e+01 ; -2.290794e+02 ; 7.369116e+02 ];
omc_error_5 = [ 8.051703e-04 ; 1.256010e-03 ; 1.413678e-03 ];
Tc_error_5  = [ 9.314563e-01 ; 8.465921e-01 ; 5.010155e-01 ];

%-- Image #6:
omc_6 = [ -1.701766e+00 ; -1.929498e+00 ; -7.917741e-01 ];
Tc_6  = [ -1.490441e+02 ; -7.960215e+01 ; 4.451486e+02 ];
omc_error_6 = [ 7.741225e-04 ; 1.253834e-03 ; 1.698279e-03 ];
Tc_error_6  = [ 5.582146e-01 ; 5.234275e-01 ; 4.286880e-01 ];

%-- Image #7:
omc_7 = [ 1.996394e+00 ; 1.931396e+00 ; 1.310790e+00 ];
Tc_7  = [ -8.307616e+01 ; -7.769524e+01 ; 4.404076e+02 ];
omc_error_7 = [ 1.482778e-03 ; 7.610995e-04 ; 1.780783e-03 ];
Tc_error_7  = [ 5.608962e-01 ; 5.115966e-01 ; 4.525418e-01 ];

%-- Image #8:
omc_8 = [ 1.961170e+00 ; 1.824198e+00 ; 1.326282e+00 ];
Tc_8  = [ -1.702794e+02 ; -1.035203e+02 ; 4.623205e+02 ];
omc_error_8 = [ 1.414863e-03 ; 7.761873e-04 ; 1.707626e-03 ];
Tc_error_8  = [ 6.132885e-01 ; 5.558431e-01 ; 5.098297e-01 ];

%-- Image #9:
omc_9 = [ -1.363901e+00 ; -1.980862e+00 ; 3.208860e-01 ];
Tc_9  = [ -2.107848e+00 ; -2.250933e+02 ; 7.289449e+02 ];
omc_error_9 = [ 9.645601e-04 ; 1.238782e-03 ; 1.596297e-03 ];
Tc_error_9  = [ 9.190366e-01 ; 8.338221e-01 ; 5.210267e-01 ];

%-- Image #10:
omc_10 = [ -1.513492e+00 ; -2.087101e+00 ; 1.880738e-01 ];
Tc_10  = [ -2.988523e+01 ; -3.003472e+02 ; 8.605054e+02 ];
omc_error_10 = [ 1.176220e-03 ; 1.408106e-03 ; 2.122549e-03 ];
Tc_error_10  = [ 1.104438e+00 ; 9.911714e-01 ; 6.915368e-01 ];

%-- Image #11:
omc_11 = [ -1.793139e+00 ; -2.065010e+00 ; -4.802063e-01 ];
Tc_11  = [ -1.512820e+02 ; -2.352881e+02 ; 7.050052e+02 ];
omc_error_11 = [ 1.055022e-03 ; 1.328785e-03 ; 2.283920e-03 ];
Tc_error_11  = [ 9.054019e-01 ; 8.487694e-01 ; 6.840616e-01 ];

%-- Image #12:
omc_12 = [ -1.839142e+00 ; -2.087535e+00 ; -5.158613e-01 ];
Tc_12  = [ -1.336743e+02 ; -1.771663e+02 ; 6.051959e+02 ];
omc_error_12 = [ 8.995790e-04 ; 1.277209e-03 ; 2.107451e-03 ];
Tc_error_12  = [ 7.711555e-01 ; 7.176821e-01 ; 5.719644e-01 ];

%-- Image #13:
omc_13 = [ -1.919022e+00 ; -2.116713e+00 ; -5.945150e-01 ];
Tc_13  = [ -1.328659e+02 ; -1.435033e+02 ; 5.450045e+02 ];
omc_error_13 = [ 8.392228e-04 ; 1.264287e-03 ; 2.072056e-03 ];
Tc_error_13  = [ 6.924561e-01 ; 6.424221e-01 ; 5.191193e-01 ];

%-- Image #14:
omc_14 = [ -1.954395e+00 ; -2.124760e+00 ; -5.847839e-01 ];
Tc_14  = [ -1.237533e+02 ; -1.370916e+02 ; 4.910906e+02 ];
omc_error_14 = [ 7.898303e-04 ; 1.239012e-03 ; 2.028490e-03 ];
Tc_error_14  = [ 6.248674e-01 ; 5.783201e-01 ; 4.659129e-01 ];

%-- Image #15:
omc_15 = [ -2.110704e+00 ; -2.253882e+00 ; -4.950597e-01 ];
Tc_15  = [ -1.993040e+02 ; -1.344612e+02 ; 4.752472e+02 ];
omc_error_15 = [ 9.114180e-04 ; 1.159878e-03 ; 2.210192e-03 ];
Tc_error_15  = [ 6.129526e-01 ; 5.736046e-01 ; 5.020571e-01 ];

%-- Image #16:
omc_16 = [ 1.886758e+00 ; 2.335939e+00 ; -1.729953e-01 ];
Tc_16  = [ -1.615402e+01 ; -1.702753e+02 ; 6.958200e+02 ];
omc_error_16 = [ 1.252575e-03 ; 1.323440e-03 ; 2.749208e-03 ];
Tc_error_16  = [ 8.717357e-01 ; 7.911864e-01 ; 5.946032e-01 ];

%-- Image #17:
omc_17 = [ -1.612964e+00 ; -1.953643e+00 ; -3.476711e-01 ];
Tc_17  = [ -1.353877e+02 ; -1.389062e+02 ; 4.903580e+02 ];
omc_error_17 = [ 7.804835e-04 ; 1.194016e-03 ; 1.682700e-03 ];
Tc_error_17  = [ 6.168320e-01 ; 5.736647e-01 ; 4.130536e-01 ];

%-- Image #18:
omc_18 = [ -1.341894e+00 ; -1.693366e+00 ; -2.975759e-01 ];
Tc_18  = [ -1.854450e+02 ; -1.577390e+02 ; 4.415836e+02 ];
omc_error_18 = [ 8.954238e-04 ; 1.160254e-03 ; 1.328880e-03 ];
Tc_error_18  = [ 5.609484e-01 ; 5.230809e-01 ; 4.012698e-01 ];

%-- Image #19:
omc_19 = [ -1.925896e+00 ; -1.838152e+00 ; -1.440606e+00 ];
Tc_19  = [ -1.066810e+02 ; -7.954567e+01 ; 3.343515e+02 ];
omc_error_19 = [ 7.704945e-04 ; 1.358526e-03 ; 1.721749e-03 ];
Tc_error_19  = [ 4.352634e-01 ; 3.991738e-01 ; 3.762842e-01 ];

%-- Image #20:
omc_20 = [ 1.895846e+00 ; 1.593082e+00 ; 1.471977e+00 ];
Tc_20  = [ -1.439836e+02 ; -8.800496e+01 ; 3.964140e+02 ];
omc_error_20 = [ 1.435172e-03 ; 7.937624e-04 ; 1.546513e-03 ];
Tc_error_20  = [ 5.313236e-01 ; 4.757669e-01 ; 4.551934e-01 ];


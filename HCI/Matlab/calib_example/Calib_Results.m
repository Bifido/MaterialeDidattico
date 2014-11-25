% Intrinsic and Extrinsic Camera Parameters
%
% This script file can be directly excecuted under Matlab to recover the camera intrinsic and extrinsic parameters.
% IMPORTANT: This file contains neither the structure of the calibration objects nor the image coordinates of the calibration points.
%            All those complementary variables are saved in the complete matlab data file Calib_Results.mat.
% For more information regarding the calibration model visit http://www.vision.caltech.edu/bouguetj/calib_doc/


%-- Focal length:
fc = [ 657.390815890036610 ; 657.758428689976430 ];

%-- Principal point:
cc = [ 302.981364468558810 ; 242.617938990990980 ];

%-- Skew coefficient:
alpha_c = 0.000000000000000;

%-- Distortion coefficients:
kc = [ -0.255842380984747 ; 0.127557575649864 ; -0.000208545264778 ; 0.000033348675723 ; 0.000000000000000 ];

%-- Focal length uncertainty:
fc_error = [ 0.346741997556188 ; 0.370930177868197 ];

%-- Principal point uncertainty:
cc_error = [ 0.705117050336511 ; 0.645212339239958 ];

%-- Skew coefficient uncertainty:
alpha_c_error = 0.000000000000000;

%-- Distortion coefficients uncertainty:
kc_error = [ 0.002705333408903 ; 0.010752575372531 ; 0.000145714796926 ; 0.000143931740603 ; 0.000000000000000 ];

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
omc_1 = [ 1.654781e+00 ; 1.651923e+00 ; -6.699940e-01 ];
Tc_1  = [ -5.919145e+02 ; -2.791434e+02 ; 2.843261e+03 ];
omc_error_1 = [ 8.233553e-04 ; 1.064047e-03 ; 1.360044e-03 ];
Tc_error_1  = [ 3.052178e+00 ; 2.814527e+00 ; 1.546377e+00 ];

%-- Image #2:
omc_2 = [ 1.849013e+00 ; 1.900564e+00 ; -3.971222e-01 ];
Tc_2  = [ -5.165389e+02 ; -5.311889e+02 ; 2.525333e+03 ];
omc_error_2 = [ 8.650897e-04 ; 1.057327e-03 ; 1.644563e-03 ];
Tc_error_2  = [ 2.725318e+00 ; 2.494964e+00 ; 1.520076e+00 ];

%-- Image #3:
omc_3 = [ 1.742393e+00 ; 2.077566e+00 ; -5.052453e-01 ];
Tc_3  = [ -4.174660e+02 ; -5.820988e+02 ; 2.584918e+03 ];
omc_error_3 = [ 7.917278e-04 ; 1.120326e-03 ; 1.700105e-03 ];
Tc_error_3  = [ 2.785878e+00 ; 2.553175e+00 ; 1.461231e+00 ];

%-- Image #4:
omc_4 = [ 1.827859e+00 ; 2.116782e+00 ; -1.103197e+00 ];
Tc_4  = [ -2.147681e+02 ; -5.162407e+02 ; 2.596998e+03 ];
omc_error_4 = [ 7.106436e-04 ; 1.160683e-03 ; 1.591950e-03 ];
Tc_error_4  = [ 2.807728e+00 ; 2.548404e+00 ; 1.177138e+00 ];

%-- Image #5:
omc_5 = [ 1.079054e+00 ; 1.922504e+00 ; -2.527470e-01 ];
Tc_5  = [ -3.074783e+02 ; -7.638580e+02 ; 2.455500e+03 ];
omc_error_5 = [ 6.941497e-04 ; 1.082332e-03 ; 1.218693e-03 ];
Tc_error_5  = [ 2.674400e+00 ; 2.431623e+00 ; 1.438974e+00 ];

%-- Image #6:
omc_6 = [ -1.701813e+00 ; -1.929289e+00 ; -7.914675e-01 ];
Tc_6  = [ -4.963361e+02 ; -2.654964e+02 ; 1.483249e+03 ];
omc_error_6 = [ 6.671577e-04 ; 1.080495e-03 ; 1.463655e-03 ];
Tc_error_6  = [ 1.602723e+00 ; 1.503384e+00 ; 1.231023e+00 ];

%-- Image #7:
omc_7 = [ 1.996751e+00 ; 1.931472e+00 ; 1.310634e+00 ];
Tc_7  = [ -2.764350e+02 ; -2.591217e+02 ; 1.467237e+03 ];
omc_error_7 = [ 1.277744e-03 ; 6.560797e-04 ; 1.535172e-03 ];
Tc_error_7  = [ 1.610192e+00 ; 1.469199e+00 ; 1.299731e+00 ];

%-- Image #8:
omc_8 = [ 1.961460e+00 ; 1.824260e+00 ; 1.326200e+00 ];
Tc_8  = [ -5.670326e+02 ; -3.452046e+02 ; 1.540225e+03 ];
omc_error_8 = [ 1.219449e-03 ; 6.692117e-04 ; 1.472320e-03 ];
Tc_error_8  = [ 1.760447e+00 ; 1.596193e+00 ; 1.464200e+00 ];

%-- Image #9:
omc_9 = [ -1.363688e+00 ; -1.980538e+00 ; 3.210324e-01 ];
Tc_9  = [ -6.254210e+00 ; -7.505352e+02 ; 2.428802e+03 ];
omc_error_9 = [ 8.314222e-04 ; 1.067777e-03 ; 1.375845e-03 ];
Tc_error_9  = [ 2.638684e+00 ; 2.394881e+00 ; 1.496551e+00 ];

%-- Image #10:
omc_10 = [ -1.513263e+00 ; -2.086815e+00 ; 1.882477e-01 ];
Tc_10  = [ -9.868256e+01 ; -1.001444e+03 ; 2.867185e+03 ];
omc_error_10 = [ 1.014073e-03 ; 1.214053e-03 ; 1.829618e-03 ];
Tc_error_10  = [ 3.171057e+00 ; 2.846857e+00 ; 1.986290e+00 ];

%-- Image #11:
omc_11 = [ -1.793084e+00 ; -2.064813e+00 ; -4.799203e-01 ];
Tc_11  = [ -5.035041e+02 ; -7.845513e+02 ; 2.349136e+03 ];
omc_error_11 = [ 9.096898e-04 ; 1.145594e-03 ; 1.969073e-03 ];
Tc_error_11  = [ 2.599615e+00 ; 2.437939e+00 ; 1.964982e+00 ];

%-- Image #12:
omc_12 = [ -1.839112e+00 ; -2.087341e+00 ; -5.155417e-01 ];
Tc_12  = [ -4.449269e+02 ; -5.907708e+02 ; 2.016566e+03 ];
omc_error_12 = [ 7.754933e-04 ; 1.100855e-03 ; 1.816573e-03 ];
Tc_error_12  = [ 2.214179e+00 ; 2.061425e+00 ; 1.642940e+00 ];

%-- Image #13:
omc_13 = [ -1.919017e+00 ; -2.116532e+00 ; -5.941675e-01 ];
Tc_13  = [ -4.422996e+02 ; -4.785380e+02 ; 1.815990e+03 ];
omc_error_13 = [ 7.233835e-04 ; 1.089613e-03 ; 1.785949e-03 ];
Tc_error_13  = [ 1.988207e+00 ; 1.845220e+00 ; 1.491081e+00 ];

%-- Image #14:
omc_14 = [ -1.954382e+00 ; -2.124574e+00 ; -5.844121e-01 ];
Tc_14  = [ -4.119856e+02 ; -4.571472e+02 ; 1.636329e+03 ];
omc_error_14 = [ 6.808279e-04 ; 1.067879e-03 ; 1.748417e-03 ];
Tc_error_14  = [ 1.794112e+00 ; 1.661054e+00 ; 1.338231e+00 ];

%-- Image #15:
omc_15 = [ -2.110766e+00 ; -2.253834e+00 ; -4.948435e-01 ];
Tc_15  = [ -6.637953e+02 ; -4.483692e+02 ; 1.583453e+03 ];
omc_error_15 = [ 7.858104e-04 ; 1.000125e-03 ; 1.905586e-03 ];
Tc_error_15  = [ 1.759602e+00 ; 1.647390e+00 ; 1.442305e+00 ];

%-- Image #16:
omc_16 = [ 1.886911e+00 ; 2.336198e+00 ; -1.735862e-01 ];
Tc_16  = [ -5.310591e+01 ; -5.677851e+02 ; 2.318539e+03 ];
omc_error_16 = [ 1.080351e-03 ; 1.141229e-03 ; 2.372366e-03 ];
Tc_error_16  = [ 2.502998e+00 ; 2.272518e+00 ; 1.708078e+00 ];

%-- Image #17:
omc_17 = [ -1.612907e+00 ; -1.953391e+00 ; -3.473510e-01 ];
Tc_17  = [ -4.507701e+02 ; -4.631912e+02 ; 1.633949e+03 ];
omc_error_17 = [ 6.726865e-04 ; 1.028936e-03 ; 1.450273e-03 ];
Tc_error_17  = [ 1.771075e+00 ; 1.647721e+00 ; 1.186095e+00 ];

%-- Image #18:
omc_18 = [ -1.341748e+00 ; -1.692553e+00 ; -2.970112e-01 ];
Tc_18  = [ -6.178935e+02 ; -5.260029e+02 ; 1.470963e+03 ];
omc_error_18 = [ 7.720283e-04 ; 9.995983e-04 ; 1.144949e-03 ];
Tc_error_18  = [ 1.610176e+00 ; 1.501954e+00 ; 1.152153e+00 ];

%-- Image #19:
omc_19 = [ -1.925984e+00 ; -1.837922e+00 ; -1.440323e+00 ];
Tc_19  = [ -3.552129e+02 ; -2.652418e+02 ; 1.113847e+03 ];
omc_error_19 = [ 6.640525e-04 ; 1.170986e-03 ; 1.483902e-03 ];
Tc_error_19  = [ 1.249454e+00 ; 1.146231e+00 ; 1.080577e+00 ];

%-- Image #20:
omc_20 = [ 1.896150e+00 ; 1.593137e+00 ; 1.471914e+00 ];
Tc_20  = [ -4.794416e+02 ; -2.934565e+02 ; 1.320575e+03 ];
omc_error_20 = [ 1.236935e-03 ; 6.843031e-04 ; 1.333351e-03 ];
Tc_error_20  = [ 1.525047e+00 ; 1.366162e+00 ; 1.307244e+00 ];


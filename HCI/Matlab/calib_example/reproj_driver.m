% Umberto Castellani
%
% Script per la riproiezione di punti 3D
% su immagini 2D
%

close all
clear all
cali = load(['calib_data.mat']);
camera = load(['Calib_Results.mat']);

% Mi concentro sull'immagine 10
% 
% Immagine
ImageIN=imread('Image10.tif');
% Punti 3D
X=cali.X_10;

% figure(1)
% plot3(X(1,:),X(2,:), X(3,:), 'r*');
% 
% % Calcolo la matrice dei parametri intrinseci:
% KK = camera.KK; % KK = [f(1) alpha_c*f(1)  c(1);  0  f(2)  c(2);  0 0 1];
% 
% % Calcolo la matrice degli estrinseci:
% R = camera.Rc_10;
% T=camera.Tc_10;
% G = [R T; 0 0 0 1];
% 
% % Calcolo la matrice di proiezione finale
% ppm = KK * [1 0 0 0
%             0 1 0 0
%             0 0 1 0] * G;
% 
% [u, v] = proj(ppm,X');
% figure(2)
% imshow(ImageIN);
% hold on
% plot(u, v, 'b+');
% 
% %Alternativa (con rimozione distorsione radiale):
om=camera.omc_10;
T=camera.Tc_10;
f=camera.fc;
c=camera.cc;
k=camera.kc;
% 
% 
% [xp,dxpdom,dxpdT,dxpdf,dxpdc,dxpdk] = project_points(X,om,T,f,c,k);
% 
% figure(3)
% imshow(ImageIN);
% hold on
% plot(xp(1,:), xp(2,:), 'r+');

% Effetto speciale:
% traslo la scacchiera verso il basso di due quadrati:
coeff=200;
XX_in=X;
%XX_in(1,:) = (coeff*ones(1,length(X))) + X(1,:);
%XX_in(3,1:length(X)/2) = (30*ones(1,length(X)/2)) + X(3,1:length(X)/2);

[xp_in,dxpdom,dxpdT,dxpdf,dxpdc,dxpdk] = project_points(XX_in,om,T,f,c,k);

for i=1 : 13
%hold on
        figure(4)
        imshow(ImageIN);
    for j=1:12
%         figure(1)
%         hold on
%         plot3(XX_in(1,j+i*12),XX_in(2,j+i*12), XX_in(3,j+i*12), 'b+');

        %hold off
        %
        figure(4)
        hold on
        plot(xp_in(1,i+j*12), xp_in(2,i+j*12), 'r+');
    end
    %hold off
pause(0.5);
end







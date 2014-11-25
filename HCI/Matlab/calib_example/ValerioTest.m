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
XX_in=X;
% %Alternativa (con rimozione distorsione radiale):
om=camera.omc_10;
T=camera.Tc_10;
f=camera.fc;
c=camera.cc;
k=camera.kc;


coeff=200;
XX_in=X;
XX_in(1,:) = (coeff*ones(1,length(X))) + X(1,:);
XX_in(3,1:length(X)/2) = (30*ones(1,length(X)/2)) + X(3,1:length(X)/2);
[xp_in,dxpdom,dxpdT,dxpdf,dxpdc,dxpdk] = project_points(XX_in,om,T,f,c,k);

figure(4)
imshow(ImageIN);

% falling
% for i=1:10
%     figure(4)
%     imshow(ImageIN);
%     figure(4)
%     hold on
%     plot(xp_in(1,61+i), xp_in(2,61+i), 'r+');
%     pause(0.05);
% end

%down
for j= 1:10
    figure(4)
    imshow(ImageIN);
    figure(4)
    hold on
    plot(xp_in(1,0+j), xp_in(2,0+j), 'r+');
end
%left
for j= 1:11
    figure(4)
    imshow(ImageIN);
    figure(4)
    hold on
    plot(xp_in(1,10 + 12*j), xp_in(2,10 + 12*j), 'r+');
end
%up
for j= 1:9
    figure(4)
    imshow(ImageIN);
    figure(4)
    hold on
    plot(xp_in(1,142 - j), xp_in(2,142 - j), 'r+');
end
%right
for j= 1:6
    figure(4)
    imshow(ImageIN);
    figure(4)
    hold on
    plot(xp_in(1,133 - 12*j), xp_in(2,133 - 12*j), 'r+');
end
for j= 1:10
    figure(4)
    imshow(ImageIN);
    figure(4)
    hold on
    plot(xp_in(1,61+j), xp_in(2,61+j), 'r+');
end

%moving on table
pause(0.3);
for i= 1:20      
    coeff=100+i*100;
    
    XX_in(3,:) = (coeff*ones(1,length(X))) + X(3,:);
    
    [xp_in,dxpdom,dxpdT,dxpdf,dxpdc,dxpdk] = project_points(XX_in,om,T,f,c,k);
    figure(4)
    imshow(ImageIN);
    figure(4)
    hold on  
    plot(xp_in(1,71), xp_in(2,71), 'r+');
end




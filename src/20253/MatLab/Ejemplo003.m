clc;
close all;
clear;

file_img = 'Frutas06r_rggb.jpg';
if ispc
    d = '\';
else
    d = '/';
end


path_img = '/Users/ibarram/Library/CloudStorage/Dropbox/GitHub/PDS_TR/data/RGGB/';

if ~exist(path_img, 'dir')
    path_img = uigetdir(pwd, 'Selecciona el directorio.');
    path_img = [path_img d];
end

imgRGGB = imread([path_img file_img]);
[lv, lu] = size(imgRGGB);
imgRGB = zeros(lv, lu, 3, 'uint8');

% Rojo
imgRGB(1:2:lv, 1:2:lu, 1) = imgRGGB(1:2:lv, 1:2:lu);
imgRGB(2:2:lv, 1:2:lu, 1) = imgRGGB(1:2:lv, 1:2:lu);
imgRGB(1:2:lv, 2:2:lu, 1) = imgRGGB(1:2:lv, 1:2:lu);
imgRGB(2:2:lv, 2:2:lu, 1) = imgRGGB(1:2:lv, 1:2:lu);

% Verde
imgRGB(2:2:lv, 1:2:lu, 2) = imgRGGB(2:2:lv, 1:2:lu);
imgRGB(1:2:lv, 2:2:lu, 2) = imgRGGB(1:2:lv, 2:2:lu);
imgRGB(2:2:lv, 2:2:lu, 2) = imgRGGB(2:2:lv, 1:2:lu);
imgRGB(1:2:lv, 1:2:lu, 2) = imgRGGB(1:2:lv, 2:2:lu);

% Azul
imgRGB(2:2:lv, 2:2:lu, 3) = imgRGGB(2:2:lv, 2:2:lu);
imgRGB(1:2:lv, 1:2:lu, 3) = imgRGGB(2:2:lv, 2:2:lu);
imgRGB(2:2:lv, 1:2:lu, 3) = imgRGGB(2:2:lv, 2:2:lu);
imgRGB(1:2:lv, 2:2:lu, 3) = imgRGGB(2:2:lv, 2:2:lu);

figure;
imshow(imgRGB);


void get_minfang(){
  const int NUMENTRIES_water=60;
  double GeV = 1e9;
  double ENERGY_water[NUMENTRIES_water] =
    { 1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV, 
      1.65333e-09*GeV, 1.67567e-09*GeV, 1.69863e-09*GeV, 1.72222e-09*GeV, 
      1.74647e-09*GeV, 1.77142e-09*GeV,1.7971e-09*GeV, 1.82352e-09*GeV, 
      1.85074e-09*GeV, 1.87878e-09*GeV, 1.90769e-09*GeV, 1.93749e-09*GeV, 
      1.96825e-09*GeV, 1.99999e-09*GeV, 2.03278e-09*GeV, 2.06666e-09*GeV,
      2.10169e-09*GeV, 2.13793e-09*GeV, 2.17543e-09*GeV, 2.21428e-09*GeV, 
      2.25454e-09*GeV, 2.29629e-09*GeV, 2.33962e-09*GeV, 2.38461e-09*GeV, 
      2.43137e-09*GeV, 2.47999e-09*GeV, 2.53061e-09*GeV, 2.58333e-09*GeV, 
      2.63829e-09*GeV, 2.69565e-09*GeV, 2.75555e-09*GeV, 2.81817e-09*GeV, 
      2.88371e-09*GeV, 2.95237e-09*GeV, 3.02438e-09*GeV, 3.09999e-09*GeV,
      3.17948e-09*GeV, 3.26315e-09*GeV, 3.35134e-09*GeV, 3.44444e-09*GeV, 
      3.54285e-09*GeV, 3.64705e-09*GeV, 3.75757e-09*GeV, 3.87499e-09*GeV, 
      3.99999e-09*GeV, 4.13332e-09*GeV, 4.27585e-09*GeV, 4.42856e-09*GeV, 
      4.59258e-09*GeV, 4.76922e-09*GeV, 4.95999e-09*GeV, 5.16665e-09*GeV, 
      5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV };
  
  double ABWFF = 1.0;
  double cm = 0.01;
  
  double ABSORPTION_water[NUMENTRIES_water] =
    {
      16.1419*cm*ABWFF,  18.278*cm*ABWFF, 21.0657*cm*ABWFF, 24.8568*cm*ABWFF, 30.3117*cm*ABWFF, 
      38.8341*cm*ABWFF, 54.0231*cm*ABWFF, 81.2306*cm*ABWFF, 120.909*cm*ABWFF, 160.238*cm*ABWFF, 
      193.771*cm*ABWFF, 215.017*cm*ABWFF, 227.747*cm*ABWFF,  243.85*cm*ABWFF, 294.036*cm*ABWFF, 
      321.647*cm*ABWFF,  342.81*cm*ABWFF, 362.827*cm*ABWFF, 378.041*cm*ABWFF, 449.378*cm*ABWFF,
      739.434*cm*ABWFF, 1114.23*cm*ABWFF, 1435.56*cm*ABWFF, 1611.06*cm*ABWFF, 1764.18*cm*ABWFF, 
      2100.95*cm*ABWFF,  2292.9*cm*ABWFF, 2431.33*cm*ABWFF,  3053.6*cm*ABWFF, 4838.23*cm*ABWFF, 
      6539.65*cm*ABWFF, 7682.63*cm*ABWFF, 9137.28*cm*ABWFF, 12220.9*cm*ABWFF, 15270.7*cm*ABWFF, 
      19051.5*cm*ABWFF, 23671.3*cm*ABWFF, 29191.1*cm*ABWFF, 35567.9*cm*ABWFF,   42583*cm*ABWFF,
      49779.6*cm*ABWFF, 56465.3*cm*ABWFF,   61830*cm*ABWFF, 65174.6*cm*ABWFF, 66143.7*cm*ABWFF,   
      64820*cm*ABWFF,   61635*cm*ABWFF, 57176.2*cm*ABWFF, 52012.1*cm*ABWFF, 46595.7*cm*ABWFF, 
      41242.1*cm*ABWFF, 36146.3*cm*ABWFF, 31415.4*cm*ABWFF, 27097.8*cm*ABWFF, 23205.7*cm*ABWFF, 
      19730.3*cm*ABWFF, 16651.6*cm*ABWFF, 13943.6*cm*ABWFF, 11578.1*cm*ABWFF, 9526.13*cm*ABWFF
    };
  
  

   //Xin Qian: proposed value for absorption length
  double ABSORPTION_water1[NUMENTRIES_water] =
    {
      22.8154*cm*ABWFF, 28.6144*cm*ABWFF, 35.9923*cm*ABWFF, 45.4086*cm*ABWFF, 57.4650*cm*ABWFF,
      72.9526*cm*ABWFF, 75*cm*ABWFF,      81.2317*cm*ABWFF, 120.901*cm*ABWFF, 160.243*cm*ABWFF,
      193.797*cm*ABWFF, 215.045*cm*ABWFF, 227.786*cm*ABWFF, 243.893*cm*ABWFF, 294.113*cm*ABWFF,
      321.735*cm*ABWFF, 342.931*cm*ABWFF, 362.967*cm*ABWFF, 378.212*cm*ABWFF, 449.602*cm*ABWFF,
      740.143*cm*ABWFF, 1116.06*cm*ABWFF, 1438.78*cm*ABWFF, 1615.48*cm*ABWFF, 1769.86*cm*ABWFF,
      2109.67*cm*ABWFF, 2304.13*cm*ABWFF, 2444.97*cm*ABWFF, 3076.83*cm*ABWFF, 4901.5*cm*ABWFF,
      6666.57*cm*ABWFF, 7873.95*cm*ABWFF, 9433.81*cm*ABWFF, 10214.5*cm*ABWFF, 10845.8*cm*ABWFF,
      15746.9*cm*ABWFF, 20201.8*cm*ABWFF, 22025.8*cm*ABWFF, 21142.2*cm*ABWFF, 15083.9*cm*ABWFF,
      11751*cm*ABWFF,   8795.34*cm*ABWFF, 8741.23*cm*ABWFF, 7102.37*cm*ABWFF, 6060.68*cm*ABWFF,
      4498.56*cm*ABWFF, 3039.56*cm*ABWFF, 2232.2*cm*ABWFF,  1938*cm*ABWFF,    1811.58*cm*ABWFF,
      1610.32*cm*ABWFF, 1338.7*cm*ABWFF,  1095.3*cm*ABWFF,  977.525*cm*ABWFF, 965.258*cm*ABWFF,
      1082.86*cm*ABWFF, 876.434*cm*ABWFF, 633.723*cm*ABWFF, 389.87*cm*ABWFF,  142.011*cm*ABWFF
    };


  double wavelength[60],lambda[60],lambda1[60];
  
  for (Int_t i=59;i!=-1;i--){
    wavelength[i] = 1240./ENERGY_water[i];
    lambda[i] = 1./(ABSORPTION_water[i]);
    lambda1[i] = 1./(ABSORPTION_water1[i]);
    cout << wavelength[i] << " " << lambda[i] << " " << lambda1[i] << endl;
  }

}

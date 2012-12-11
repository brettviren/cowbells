void plot(){
  TChain *T = new TChain("noLC_hit","noLC_hit");
  T->Add("electron_noLC1_1.root");
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->SetFillColor(10);
  //  T->Draw("wavelength");
  gStyle->SetOptStat(0);
  TH1F *h1 = new TH1F("h1","h1",100,0.,100.);
  T->Project("h1","corr_time","neve==12");
  //h1->SetTitle("K^{+} ->  #mu^{+}");
  h1->SetTitle("K^{+} ->  #pi^{0} + #pi^{+} -> #gammas + #mu^{+} --> e^{+}");
  h1->Draw();
  h1->SetXTitle("Corrected Time at Vertex (ns)");
}

#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"

#include <vector>
using std::vector;

TCanvas* canvas = new TCanvas("c","canvas");


vector<double> one_fit(bool do_fit=true);
vector<double> one_fit(bool do_fit)
{
    TH1F* h = new TH1F("h","h", 20, -10, 10);
    h->FillRandom("gaus");
    vector<double> ret;
    if (do_fit) {
        h->Fit("gaus","");
        TF1* g = h->GetFunction("gaus");
        h->Draw();
        canvas->Modified();
        canvas->Update();
        for (int ind=0; ind < 3; ++ind) {
            ret.push_back(g->GetParameter(ind));
        }
    }
    else {
        for (int ind=0; ind < 3; ++ind) {
            ret.push_back(0.0);
        }
    }
    
    ret.push_back(h->GetMean());
    ret.push_back(h->GetRMS());
    delete h;
    return ret;
}

TGraph* tgraph(const char* name, const char* title, int color)
{
    TGraph* t = new TGraph();
    t->SetName(name);
    t->SetTitle(title);
    t->SetLineColor(color);
    return t;
}

void test_many_fits(int ntries = 100, bool do_fit = true);
void test_many_fits(int ntries, bool do_fit)
{

    TH1F* means1d = new TH1F("means1d","Fitted means", 200, -10, 10);
    TH1F* sigmas1d = new TH1F("sigmas1d","Fitted sigmas", 100, 0, 10);

    TGraph* means = tgraph("means","Fitted means vs try", 1);
    TGraph* avgs = tgraph("avgs","Histogram average vs try", 2);

    TGraph* sigmas = tgraph("sigmas","Fitted sigmas vs try", 1);
    TGraph* rmses = tgraph("rmses","Histogram RMSs vs try", 2);

    for (int count=0; count < ntries; ++count) {
        vector<double> ret = one_fit(do_fit);

        if (ret[0] != 0) {
            means1d->Fill(ret[1]);
            means->SetPoint(count,count,ret[1]);
            sigmas1d->Fill(ret[2]);
            sigmas->SetPoint(count,count,ret[2]);
        }
        avgs->SetPoint(count,count,ret[3]);
        rmses->SetPoint(count,count,ret[4]);
    }
    canvas->Clear();
    canvas->Divide(2,2);

    canvas->cd(1);
    means1d->Draw();
    canvas->cd(2);
    avgs->Draw("AL");
    if (do_fit) {
        means->Draw("");
    }

    canvas->cd(3);
    sigmas1d->Draw();
    canvas->cd(4);
    rmses->Draw("AL");
    if (do_fit) {
        sigmas->Draw("");
    }

    if (do_fit) {
        canvas->Print("test_fit.pdf");
    }
    else {
        canvas->Print("test_hist.pdf");
    }
}
void test_fit()
{
    test_many_fits(1000,true);
    test_many_fits(10000,false);
}

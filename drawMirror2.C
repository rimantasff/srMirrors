#include <TCanvas.h>
#include <TH3D.h>
#include <TProfile2D.h>
#include <iostream>
#include <TString.h>
#include <TStyle.h>

/*
  A simple macro tho visualize beam reflection between two mirrors,
  one mirror is above the other (in z direction).
  One pair of mirrors is stationary (move=0), another pair is moving
  along the x-axis (move=1).
  To reduce calculation, the beam and the mirrors are displayed using
  TH3D histogram.

  The macro creates plots for the pair of mirrors at a given
  time (specified by time_pos>=0).

  If doSave=1, the canvases are saved to a hard-coded directory, which is
  specified by outPicDir.
*/

const TString outPicDir="dir-pics2/";

void drawMirror2(int time_pos=0, int doSave=0, int move=1)
{
  const double tMax=100.;
  TH3D *h3mirror= new TH3D("h3mirror","h3mirror;x;z;time",
			   110,-5.,105.,200,-5.,5.,
			   55,-5.,tMax);
  h3mirror->SetStats(0);

  for (int ibx=0; ibx<h3mirror->GetNbinsX(); ibx++) {
    for (int iby=0; iby<h3mirror->GetNbinsY(); iby++) {
      for (int ibt=0; ibt<h3mirror->GetNbinsZ(); ibt++) {
	h3mirror->SetBinContent(ibx,iby,ibt,0);
	h3mirror->SetBinError(ibx,iby,ibt,0);
      }
    }
  }

  int nTimePoints=20; // orig 20 then 5
  double tMin=0;
  double tH=(50-tMin)/(nTimePoints-1);
  

  int nMirrXPoints=5;
  double yMirrPos1=-1.5, yMirrPos2=1.5;
  double tPos= tMin + time_pos*tH;
  double xVelPerTime=2*move;
  int nBeamTicks=4;
  //double beamSpeed=(yMirrPos2-yMirrPos1)/(nBeamTicks-1);
  double xMirrMin=xVelPerTime*(tPos-1), xMirrMax=xVelPerTime*(tPos+1);
  double xMirrH=(nMirrXPoints==1) ? 0 : (xMirrMax-xMirrMin)/(nMirrXPoints-1);

  double beamYLen= (yMirrPos2-yMirrPos1)/8;
  double nBeamFullSize=50; // number of pixels for the beam
  TH3D* h3beam= (TH3D*)h3mirror->Clone("h3beam");
  h3beam->SetTitle("h3beam");
  h3beam->Reset();
  int beamTick=time_pos%nBeamTicks;
  double beamHeadWeight=5;
  if (beamTick==0) {
    // emitting
    double beamX= (xMirrMin+xMirrMax)/2;
    double beamYMin= yMirrPos1;
    int nBeamSize= nBeamFullSize;
    double beamYMax= nBeamSize/double(nBeamFullSize)*beamYLen+yMirrPos1;
    double beamH= (beamYMax-beamYMin)/(nBeamSize-1);
    for (int ib=0; ib<nBeamSize; ib++) {
      h3beam->Fill(beamX, beamYMin+ib*beamH, tPos, 0.1);
    }
    h3beam->Fill(beamX, beamYMax, tPos, beamHeadWeight);
  }
  else if ((beamTick==1) || (beamTick==3)) {
    // half-way
    double beamX= (xMirrMin+xMirrMax)/2;
    double beamCenter= (yMirrPos1+yMirrPos2);
    double beamYMin= beamCenter;
    double beamYMax= beamCenter + beamYLen * ((beamTick==1)? 1 : -1);
    int nBeamSize= nBeamFullSize;
    double beamH= (beamYMax-beamYMin)/(nBeamSize-1);
    for (int ib=0; ib<nBeamSize; ib++) {
      h3beam->Fill(beamX, beamYMin+ib*beamH, tPos, 0.1);
    }
    h3beam->Fill(beamX, beamYMax, tPos, beamHeadWeight);
  }
  else if (beamTick==2) {
    // reflected
    double beamX= (xMirrMin+xMirrMax)/2;
    double beamYMin= yMirrPos2;
    double beamYMax= yMirrPos2 - beamYLen;
    int nBeamSize= nBeamFullSize;
    double beamH= (beamYMax-beamYMin)/(nBeamSize-1);
    for (int ib=0; ib<nBeamSize; ib++) {
      h3beam->Fill(beamX, beamYMin+ib*beamH, tPos, 0.1);
    }
    h3beam->Fill(beamX, beamYMax, tPos, beamHeadWeight);
  }


  for (int i=0; i<nMirrXPoints; i++) {
    double x= xMirrMin + xMirrH * i;
    h3mirror->Fill(x,yMirrPos1, tPos,10);
    h3mirror->Fill(x,yMirrPos2, tPos,10);
  }

  TCanvas *cx= new TCanvas("cx","cx",800,800);
  h3mirror->SetTitle("");
  h3mirror->SetMarkerColor(kBlue);
  h3mirror->Draw("LEGO");
  h3beam->SetMarkerColor(kRed);
  h3beam->SetLineColor(kRed);
  h3beam->Draw("LEGOsame");
  cx->Update();


  TH2D *prMirr_xy= (TH2D*)h3mirror->Project3D("yx");
  TH2D *prBeam_xy= (TH2D*)h3beam->Project3D("yx");
  prBeam_xy->SetMarkerStyle(20);
  prBeam_xy->SetMarkerSize(0.8);
  TH2D *prMirr_xt= (TH2D*)h3mirror->Project3D("zx");
  TH2D *prBeam_xt= (TH2D*)h3beam->Project3D("zx"); 
  prBeam_xt->SetMarkerStyle(20);
  prBeam_xt->SetMarkerSize(0.8);
  TH2D *prMirr_yt= (TH2D*)h3mirror->Project3D("zy");
  TH2D *prBeam_yt= (TH2D*)h3beam->Project3D("zy"); 
  prBeam_yt->SetMarkerStyle(20);
  prBeam_yt->SetMarkerSize(0.8);

  gStyle->SetOptStat(0);
  TCanvas *cp = new TCanvas("cp","cp",900,900);
  cp->Divide(2,2);
  cp->cd(1);
  h3mirror->Draw("LEGO");
  h3beam->Draw("LEGOsame");
  cp->cd(2);
  prMirr_yt->Draw("P");
  prBeam_yt->Draw("Psame");
  cp->cd(3);
  prMirr_xy->Draw("P");
  prBeam_xy->Draw("Psame");
  cp->cd(4);
  prMirr_xt->Draw("P");
  prBeam_xt->Draw("Psame");
  cp->Modified();
  cp->Update();

  if (doSave) {
    TString form= (time_pos<10) ? "frame_0%d.png" : "frame_%d.png";
    if (xVelPerTime==0) form.Prepend("fixed_");
    form.Prepend(Form("ntp%d_",nTimePoints));
    TString fname=Form(form,time_pos);
    fname.Prepend(outPicDir);
    cx->SaveAs(fname);
  }

  if (doSave) {
    TString form= (time_pos<10) ? "cpframe_0%d.png" : "cpframe_%d.png";
    if (xVelPerTime==0) form.Prepend("fixed_");
    form.Prepend(Form("ntp%d_",nTimePoints));
    TString fname=Form(form,time_pos);
    fname.Prepend(outPicDir);
    cp->SaveAs(fname);
  }
}

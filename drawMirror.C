//#include <TApplication.h>
#include <TCanvas.h>
#include <TH3D.h>
#include <iostream>
#include <TString.h>

void drawMirror(int time_pos=0, int doSave=0, int move=1)
{
  const double tMax=100.;
  TH3D *h3mirror= new TH3D("h3mirror","h3mirror;x;y;t",
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

  int nTimePoints=20; // orig 20
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

  if (doSave) {
    TString form= (time_pos<10) ? "frame_0%d.png" : "frame_%d.png";
    if (xVelPerTime==0) form.Prepend("fixed_");
    form.Prepend(Form("ntp%d_",nTimePoints));
    TString fname=Form(form,time_pos);
    cx->SaveAs(fname);
  }
}

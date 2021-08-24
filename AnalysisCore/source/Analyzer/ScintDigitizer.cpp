#include "Analyzer/ScintDigitizer.h"
#include "Randomize.hh"
#include "DataIO/PCTEvent.h"
#include "Core/GeometryHelper.h"

ScintDigitizer::ScintDigitizer() {
    name = "ScintDigitizer";
    description = "Fast Digitization by Jifeng Hu";
    RegisterParameter("ScintRefractionIndex", &nBC408, 1.58, "Refraction Index of BC408 Scintillator", "double");
    RegisterParameter("ADCUnit", &cADC_per_MeV, 1000., "ADC count per MeV", "double");
    RegisterParameter("cATTEN_LENGTH", &cATTEN_LENGTH, 380., "attenuation length in mm", "double");
    RegisterParameter("cSCALE_LENGTH", &cSCALE_LENGTH, 1000., "smear normalization length", "double");

}

void ScintDigitizer::Begin() {
    cBC408 = 299.792458 / nBC408;
    fNumOfDigisProduced = 0;
}

void ScintDigitizer::ProcessEvt(PCTEvent *evt) {
    //cout << evt->getEventId() << endl;
    const double adc_noise_a = 0.;
    const double adc_noise_b = 0.;

    auto det_name_list = std::vector<TString>({"Scintillator", "Telescope"});

    for (const auto &det_name : det_name_list) {
        auto data_vec = evt->GetData(det_name, DetectorHit_DataType::COL);
        size_t nHits = data_vec->size();

        for (size_t i = 0; i < nHits; ++i) {
            PCTXData *item = (*data_vec)[i];
            double time = item->getT();//get the occurrence time [ns]
            double de = item->getE();//get the energy deposition
            if (de < 0.01) continue;//discard very small energy depostion: 0.01 MeV

            double x = item->getX();
            double y = item->getY();
            double z = item->getZ();
            //        double dis_a = PCTGeoSvc::Handle()->Distance2Lend(x,y,z, true);
            //        double dis_b = PCTGeoSvc::Handle()->Distance2Rend(x,y,z, false);
            double dis_a = pGeo->GetDisToSuf(TVector3(x, y, z), det_name, item->getCellId(), surface::y_plus);
            double dis_b = pGeo->GetDisToSuf(TVector3(x, y, z), det_name, item->getCellId(), surface::y_minus);
            //
            double width_a = 0.1 + 0.08 * dis_a / cSCALE_LENGTH;//ns
            double width_b = 0.1 + 0.08 * dis_b / cSCALE_LENGTH;//ns
            double dt_a = G4RandGauss::shoot(0, width_a);//
            double dt_b = G4RandGauss::shoot(0, width_b);//

            item->setADC((de * exp(-dis_a / cATTEN_LENGTH) + adc_noise_a) * cADC_per_MeV, true);
            item->setTDC(time + dis_a / cBC408 + dt_a, true);
            item->setADC((de * exp(-dis_b / cATTEN_LENGTH) + adc_noise_b) * cADC_per_MeV, false);
            item->setTDC(time + dis_b / cBC408 + dt_b, false);

            ++fNumOfDigisProduced;
        }
    }

}

void ScintDigitizer::CheckEvt(PCTEvent *) {
}

void ScintDigitizer::End() {
    std::cout << __FILE__ << "::" << __LINE__
              << "::ScintDigitizer::End(), generate nDigi = "
              << fNumOfDigisProduced << std::endl;
}
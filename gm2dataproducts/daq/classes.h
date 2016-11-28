// classes.h
// How ROOT knows how to deal with our data products
//

#include <string>
#include <sys/types.h>

#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"

//Trigger art record
#include "gm2dataproducts/daq/TriggerArtRecord.hh"
template class art::Wrapper< gm2common::TriggerArtRecord >;
template class art::Ptr< gm2common::TriggerArtRecord >;

//GPS art record
#include "gm2dataproducts/daq/GPSArtRecord.hh"
template class art::Wrapper< gm2common::GPSArtRecord >;
template class art::Ptr< gm2common::GPSArtRecord >;

//MIDAS art records...
#include "gm2dataproducts/daq/MidasEventHeaderArtRecord.hh"
#include "gm2dataproducts/daq/MidasODBArtRecord.hh"
#include "gm2dataproducts/daq/MidasEventArtRecord.hh"

//MIDAS word types
template class std::vector<uint16_t>;
template class std::vector<int16_t>;
template class std::vector<uint32_t>;
template class std::vector<uint64_t>;

//MIDAS ODB
template class std::vector< gm2midastoart::MidasODBArtRecord >;
template class art::Wrapper< gm2midastoart::MidasODBArtRecord >;

//MIDAS event info
template class std::vector<std::string>;
template class art::Wrapper< gm2midastoart::MidasEventHeaderArtRecord >;

//Master trigger bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::MasterTriggerBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::MasterTriggerBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::MasterTriggerBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::MasterTriggerBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::MasterTriggerBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::MasterTriggerBank> > >;

//GPS bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::GPSBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::GPSBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::GPSBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::GPSBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::GPSBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::GPSBank> > >;

//Tracker bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerBank> > >;

//Tracker (GLIB DAQ) bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerGLIBBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerGLIBBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerGLIBBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerGLIBBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerGLIBBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerGLIBBank> > >;

//Tracker per readout slow data bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerSlowBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerSlowBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerSlowBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerSlowBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerSlowBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::TrackerSlowBank> > >;

//Calo header bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloHeaderBank> > >;


//Calo raw bank                                                                                                        
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloRawBank> > >;


//Calo T-method bank                                                                                                            
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloTBank> > >;


//Calo Q-method-decimated bank                                                                               
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCQBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCQBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCQBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCQBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCQBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCQBank> > >;


//Calo Q-method-histogram bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCHBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCHBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCHBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCHBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCHBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::CaloCHBank> > >;

//Galil bank
template class gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank>;
template class std::vector< gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank> >;
template class art::Wrapper< gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank> >;
template class art::Wrapper< std::vector<gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank> > >;
template class art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank> >;
template class std::vector< art::Ptr< gm2midastoart::MidasEventArtRecord<gm2midastoart::GalilBank> > >;

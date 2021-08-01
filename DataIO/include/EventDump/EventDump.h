//
// Created by Zhang Yulei on 7/31/21.
//

#ifndef PHANTOM_COLLECTIONMAP_H
#define PHANTOM_COLLECTIONMAP_H

template<class T>
class CollectionMap {

};


bool EventDump::GetEntry(long long i) {
    if (!t) return false;
    Long64_t ientry = t->LoadTree(i);
    if (ientry < 0) return false;
    t->GetEntry(i);
    return true;
}

void EventDump::DumpEvent() {
    if (event_number < 0) {
        for (long long i = 0; i < total_entries; ++i) {
            GetEntry(i);
            evt->ListAllCollections(Form("Event: %lld", i));
            cout<<endl;
        }
    }
    else {
        GetEntry(event_number);
        evt->ListAllCollections(Form("Event: %lld", event_number));

        /* Print detailed information */
        evt->PrintDetails();
    }
}

#endif //PHANTOM_COLLECTIONMAP_H

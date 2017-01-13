/*
 * ofxLibdcSettings.h
 *
 *  Created on: 10/01/2017
 *      Author: avilleret
 */

#pragma once

#include "ofConstants.h"
#include <map>
#include <dc1394/dc1394.h>
#include "ofxDatGui.h"
#include "ofxDatGuiSettings.h"

class ofxLibdcSettings {
public:
    ofxLibdcSettings();
    virtual ~ofxLibdcSettings();

    bool setup(dc1394camera_t *device);
    bool set(string name, int value);
    void save(string filename);
    void save(){save(m_filename);}
    void load(string filename);
    void load(){load(m_filename);}
    string getFilename(){return m_filename;}
    void setFilename(string file){
        m_filename = file;
        return;
    }

    ofxDatGuiComponent* operator[](string name){
        for ( auto p : parameters->children ){
            if ( p->is(name) ) return p;
        }
        ofLogError(LOG_NAME) << "parameter " << name << " doesn't exist returning null pointer";
        return nullptr;
    }

    ofxDatGuiFolder *parameters;
    ofxDatGuiSettings guisettings;

    static string LOG_NAME;

private:
    vector<int> ids;
    dc1394camera_t *m_dccamera;

    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    string m_filename;

};

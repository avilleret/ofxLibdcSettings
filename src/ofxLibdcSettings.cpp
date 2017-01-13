/*
 * ofxLibdcSettings.cpp
 *
 *  Created on: 03/03/2012
 *      Author: arturo
 */

#include "ofxLibdcSettings.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/types.h>
#include "ofLog.h"

string ofxLibdcSettings::LOG_NAME = "ofxLibdcSettings";

ofxLibdcSettings::ofxLibdcSettings() : parameters(nullptr), m_filename("libdcSettings.xml") {
}

ofxLibdcSettings::~ofxLibdcSettings() {
    if (parameters) delete parameters;
}

void ofxLibdcSettings::save(string filename){
    m_filename = filename;
    guisettings.save(filename, parameters);
}

void ofxLibdcSettings::load(string filename){
    guisettings.load(filename, parameters);
}

void ofxLibdcSettings::onButtonEvent(ofxDatGuiButtonEvent e)
{
    set(e.target->getName(),0);
}

void ofxLibdcSettings::onToggleEvent(ofxDatGuiToggleEvent e)
{
    set(e.target->getName(),e.checked);
}

void ofxLibdcSettings::onSliderEvent(ofxDatGuiSliderEvent e)
{
    set(e.target->getName(),e.value);
}

void ofxLibdcSettings::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onTextInputEvent not implemented" << endl;
}

void ofxLibdcSettings::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    set(e.target->getName(), e.child);
}

bool ofxLibdcSettings::setup(dc1394camera_t *device){

    m_dccamera = device;

    ofSetLogLevel(OF_LOG_VERBOSE);

    parameters = new ofxDatGuiFolder("dc1394 parameters of " + string(m_dccamera->model));

    parameters->onButtonEvent(this, &ofxLibdcSettings::onButtonEvent);
    parameters->onToggleEvent(this, &ofxLibdcSettings::onToggleEvent);
    parameters->onSliderEvent(this, &ofxLibdcSettings::onSliderEvent);
    parameters->onTextInputEvent(this, &ofxLibdcSettings::onTextInputEvent);
    parameters->onDropdownEvent(this, &ofxLibdcSettings::onDropdownEvent);

    dc1394featureset_t features;
    dc1394_feature_get_all(m_dccamera, &features);

    for (int i=0; i<DC1394_FEATURE_NUM; i++){
        dc1394feature_info_t *feat = &features.feature[i];
        if (feat->available){
            ofxDatGuiSlider * slider = parameters->addSlider(dc1394_feature_get_string(feat->id) , feat->min, feat->max, feat->value);
            slider->setPrecision(0);
        }
        dc1394feature_modes_t modes;
        dc1394_feature_get_modes(m_dccamera, feat->id, &modes);
        if(modes.num > 1){
            vector<string> opts;
            for (int i=0; i<modes.num; i++){
                string name;
                switch(modes.modes[i]){
                case DC1394_FEATURE_MODE_MANUAL:
                    name="Manual";
                    break;
                case DC1394_FEATURE_MODE_AUTO:
                    name="Auto";
                    break;
                case DC1394_FEATURE_MODE_ONE_PUSH_AUTO:
                    name="One push auto";
                    break;
                default:
                    break;
                }
                opts.push_back(name);
            }
            parameters->addDropdown(string(dc1394_feature_get_string(feat->id)+string("Modes")), opts);
        }
    }

    parameters->expand();

    return true;

}

bool ofxLibdcSettings::set(string name, int value){

    dc1394featureset_t features;
    dc1394_feature_get_all(m_dccamera, &features);

    for (int i=0; i<DC1394_FEATURE_NUM; i++){
        dc1394feature_info_t *feat = &features.feature[i];
        if (!feat->available) continue;
        if (dc1394_feature_get_string(feat->id) == name){
            if (dc1394_feature_set_value(m_dccamera, feat->id, value) == DC1394_SUCCESS){
                return true;
            } else return false;
        } else if ( string(dc1394_feature_get_string(feat->id) + string("Modes")) == name) {
            dc1394feature_mode_t mode = DC1394_FEATURE_MODE_MANUAL;
            ofxDatGuiDropdown *menu = static_cast<ofxDatGuiDropdown*> (parameters->getComponent(ofxDatGuiType::DROPDOWN, name));
            if (menu){
                string label = menu->children[value]->getLabel();
                if (label == "Auto") mode = DC1394_FEATURE_MODE_AUTO;
                else if (label == "One push auto") mode = DC1394_FEATURE_MODE_ONE_PUSH_AUTO;
            }
            if (dc1394_feature_set_mode(m_dccamera, feat->id, mode) == DC1394_SUCCESS){
                return true;
            } else return false;
        }
    }

    return true;
}

import { Injectable } from '@angular/core';
import { HttpClient, HttpParams  } from  '@angular/common/http';
import { Storage } from  '@ionic/storage';
import { WifiWizard2 } from '@ionic-native/wifi-wizard-2/ngx';
@Injectable({
  providedIn: 'root'
})
export class ArduinoLocalService {
  LOCAL_ADDRESS:  string  =  'http://192.168.1.4/?ssid=';

  constructor(
    private  httpClient:  HttpClient,
    private wifiWizard2: WifiWizard2
    ) { }

  async getCurrentNetwork() {
    return await this.wifiWizard2.getConnectedSSID()
  }

  sendWifiToArduino(ssid: string, pwd: string, api_key:string) {
    let url: string = `${this.LOCAL_ADDRESS}${ssid}&?pwd=${pwd}&?api=${api_key}!`
    this.httpClient.get(url);
  }
}

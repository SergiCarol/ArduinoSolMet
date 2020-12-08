import { Component, OnInit, Input } from '@angular/core';
import { Router } from  "@angular/router";
import { Arduino } from '../connector/arduinos';
import { ArduinoData } from '../connector/arduino-data';
import { interval, Subscription } from 'rxjs';
import { ConnectService } from '../connector/connect.service';
import { AuthResponse } from  '../auth/auth-response';

@Component({
  selector: 'app-arduino-data',
  templateUrl: './arduino-data.page.html',
  styleUrls: ['./arduino-data.page.scss'],
})
export class ArduinoDataPage implements OnInit {
  @Input() data: ArduinoData;
  arduino: Arduino;
  subscription: Subscription;
  user: AuthResponse;

  constructor(
    private  connector:  ConnectService,
    private  router:  Router,
  ) { }

  ngOnInit() {
    this.arduino = this.router.getCurrentNavigation().extras.state.arduino as Arduino;
    this.user = this.router.getCurrentNavigation().extras.state.user as AuthResponse;
    console.log("User in data", this.user)
    this.getData();
    const source = interval(5000);
    this.subscription = source.subscribe(val => this.getData());
  }

  getData() {
    // Connect to server and get Data
    this.connector.getData(this.user.api_key, this.arduino.api_key).subscribe(res =>{
        this.data = res;
      });
  }

  get Temperature() {
    return (this.data && this.data.temperature) ? this.data.temperature : null
  }
  get Humidity() {
    return (this.data && this.data.humidity) ? this.data.humidity : null
  }
  get WaterTemp() {
    return (this.data && this.data.water_temp) ? this.data.water_temp : null
  }
  get WaterPH() {
    return (this.data && this.data.water_ph) ? this.data.water_ph : null
  }
  get WaterElectrodes() {
    return (this.data && this.data.water_electrodes) ? this.data.water_electrodes : null
  }
  ngOnDestroy() {
    this.subscription && this.subscription.unsubscribe();
  }

  editServices() {
    this.router.navigate(['arduino-services'], {state: {
      arduino: this.arduino,
      user: this.user}});
  }

  goRegister() {
    this.subscription && this.subscription.unsubscribe();
    this.router.navigate(['add-arduino-register'], {state: {
      user: this.user}});
  }
}

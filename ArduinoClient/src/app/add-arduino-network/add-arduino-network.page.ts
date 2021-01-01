import { Component, Input, OnInit } from '@angular/core';
import { ArduinoLocalService } from '../connector/arduino-local.service';
import { AuthResponse } from  '../auth/auth-response';
import { Router } from  "@angular/router";
import { AuthService } from  '../auth/auth.service';

@Component({
  selector: 'app-add-arduino-network',
  templateUrl: './add-arduino-network.page.html',
  styleUrls: ['./add-arduino-network.page.scss'],
})
export class AddArduinoNetworkPage implements OnInit {

  user: AuthResponse;
  @Input() current_network: string;
  @Input() password: string;


  constructor(
    private  connector:  ArduinoLocalService,
    private  router:  Router,
    private  authserv: AuthService
  ) { }
  ngOnInit() {
    this.user = this.router.getCurrentNavigation().extras.state.user as AuthResponse;
    this.connector.getCurrentNetwork().then(res => {
      this.current_network = res;
    });
    console.log(this.current_network)
  }

  connectArduino() {
    this.connector.sendWifiToArduino(this.current_network, this.password, this.user.api_key)
  }
}

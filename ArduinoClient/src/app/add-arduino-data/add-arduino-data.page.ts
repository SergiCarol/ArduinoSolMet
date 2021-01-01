import { Component, Input, OnInit } from '@angular/core';
import { ArduinoLocalService } from '../connector/arduino-local.service';
import { AuthResponse } from  '../auth/auth-response';
import { Router } from  "@angular/router";
import { AuthService } from  '../auth/auth.service';

@Component({
  selector: 'app-add-arduino-data',
  templateUrl: './add-arduino-data.page.html',
  styleUrls: ['./add-arduino-data.page.scss'],
})
export class AddArduinoDataPage implements OnInit {

  user: AuthResponse;
  @Input() current_network: string;


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
}

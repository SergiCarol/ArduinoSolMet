import { Component, OnInit } from '@angular/core';
import { ConnectService } from '../connector/connect.service';
import { AuthResponse } from  '../auth/auth-response';
import { Router } from  "@angular/router";
import { AuthService } from  '../auth/auth.service';

@Component({
  selector: 'app-add-arduino-register',
  templateUrl: './add-arduino-register.page.html',
  styleUrls: ['./add-arduino-register.page.scss'],
})
export class AddArduinoRegisterPage implements OnInit {
  user: AuthResponse;
  name: string = "";
  constructor(
    private  connector:  ConnectService,
    private  router:  Router,
    private  authserv: AuthService

  ) { }

  ngOnInit() {
    this.user = this.router.getCurrentNavigation().extras.state.user as AuthResponse;
    if (this.user == null) {
      let tmp_user = this.authserv.isLoggedIn()
      this.user.api_key = tmp_user;
    }
    console.log("User", this.user);
  }

  addArduino(form){
    console.log("Submited form", this.name);
    this.connector.registerArduino(this.user.api_key, this.name).subscribe(res =>{
      console.log("Res", res)
    });
  }
}

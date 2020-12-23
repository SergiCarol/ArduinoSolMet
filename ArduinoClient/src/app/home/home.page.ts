import { Component, Input } from '@angular/core';
import { Router, ActivatedRoute } from  "@angular/router";
import { Arduinos, Arduino } from '../connector/arduinos';
import { ConnectService } from '../connector/connect.service';
import { AuthService } from '../auth/auth.service';
import { AuthResponse } from  '../auth/auth-response';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {
  @Input() arduinos: Arduinos;
  api_key: AuthResponse;

  constructor(
    private  connector:  ConnectService,
    private  authService:  AuthService,
    private  router:  Router,
    private  route: ActivatedRoute) { }

  ngOnInit() {
    this.api_key = this.router.getCurrentNavigation().extras.state as AuthResponse;
    if (this.api_key == null){
      this.api_key = {
        api_key: this.authService.isLoggedIn()
      };
    }
    this.connector.getArduinos(this.api_key).subscribe((res) =>
      this.arduinos = res 
    );
    console.log(this.arduinos)
  }

  select(arduino: Arduino) {
    this.router.navigate(['arduino-data'], {state: {
      arduino: arduino,
      user: this.api_key}});
  }

  editArduino(arduino: Arduino) {
    this.router.navigate(['arduino-services'], {state: {
      arduino: arduino,
      user: this.api_key}});
  }

  addArduino() {
    this.router.navigate(['add-arduino-network'], {state: {
      user: this.api_key}});
  }
}

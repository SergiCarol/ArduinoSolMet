import { Component, OnInit } from '@angular/core';
import { Router } from  "@angular/router";
import { AuthService } from '../auth/auth.service';
import { AuthResponse } from  '../auth/auth-response';

@Component({
  selector: 'app-login',
  templateUrl: './login.page.html',
  styleUrls: ['./login.page.scss'],
})
export class LoginPage implements OnInit {

  constructor(private  authService:  AuthService, private  router:  Router) { }
  ngOnInit() {
    let api_key: string = this.authService.isLoggedIn();
    let auth: AuthResponse = {
      'api_key': api_key
    };
    this.authService.login_api(auth).subscribe((res)=>{
      if (res.api_key == api_key) {
        this.router.navigate(['home'], {state: res});
      }
    });
  }

  login(form){
    this.authService.login(form.value).subscribe((res)=>{
      localStorage.setItem('api_key', res.api_key);
      this.router.navigate(['home'], {state: res});
    });
  }
}

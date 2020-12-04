import { Component, Input } from '@angular/core';
import { Router, ActivatedRoute } from  "@angular/router";
import { Arduinos } from '../connector/arduinos';
import { ConnectService } from '../connector/connect.service';
import { AuthService } from '../auth/auth.service';
import { AuthResponse } from  '../auth/auth-response';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {
  @Input() data: Arduinos;
  api_key: string;

  constructor(
    private  connector:  ConnectService,
    private  authService:  AuthService,
    private  router:  Router,
    private  route: ActivatedRoute) { }

  ngOnInit() {
    let api_key: AuthResponse = this.router.getCurrentNavigation().extras.state as AuthResponse;
    console.log(api_key);
    this.connector.getArduinos(api_key).subscribe((res) =>
      this.data = res 
    );
  }
}

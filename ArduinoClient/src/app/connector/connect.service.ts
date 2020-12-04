import { Injectable } from  '@angular/core';
import { HttpClient, HttpParams  } from  '@angular/common/http';
import { tap } from  'rxjs/operators';
import { Observable, BehaviorSubject } from  'rxjs';
import { AuthService } from '../auth/auth.service';
import { AuthResponse } from  '../auth/auth-response';
import { Arduinos } from  './arduinos';
import { ArduinoData } from  './arduino-data';

import { Storage } from  '@ionic/storage';
@Injectable({
  providedIn: 'root'
})
export class ConnectService {
  SERVER_ADDRESS:  string  =  'http://127.0.01:5000';
  constructor(private  httpClient:  HttpClient, private  storage:  Storage) { }

  getArduinos(user: AuthResponse): Observable<Arduinos>{
    let params = new HttpParams().set('api_key', user.api_key)

    return this.httpClient.get<Arduinos>(`${this.SERVER_ADDRESS}/get_arduinos`, {params: params}).pipe(
      tap(async (res:  Arduinos ) => {
        console.log(res);
        if (res) {
          return res;
        }
      })
    
    );
  }

  getData(api_key: string, arduino_key:string): Observable<ArduinoData>{
    let params = new HttpParams();

    params = params.append('api_key', api_key);
    params = params.append('arduino_key', arduino_key);

    return this.httpClient.get<ArduinoData>(`${this.SERVER_ADDRESS}/get`,{params: params}).pipe(
      tap(async (res:  ArduinoData ) => {
        if (res) {
          console.log(res);
          return res;
        }
      })
    )
  }

}

import { Injectable } from  '@angular/core';
import { HttpClient, HttpParams  } from  '@angular/common/http';
import { tap } from  'rxjs/operators';
import { Observable, BehaviorSubject } from  'rxjs';
import { AuthService } from '../auth/auth.service';
import { AuthResponse } from  '../auth/auth-response';
import { Arduinos } from  './arduinos';
import { ArduinoData } from  './arduino-data';
import { Services, Service } from  './services';
import { Storage } from  '@ionic/storage';

@Injectable({
  providedIn: 'root'
})

export class ConnectService {
  SERVER_ADDRESS:  string  =  'http:/157.230.107.10:5000';
  constructor(private  httpClient:  HttpClient, private  storage:  Storage) { }

  getArduinos(user: AuthResponse): Observable<Arduinos>{
    let params = new HttpParams().set('api_key', user.api_key)

    return this.httpClient.get<Arduinos>('http://157.230.107.10:5000/get_arduinos', {params: params}).pipe(
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

    return this.httpClient.get<ArduinoData>('http://157.230.107.10:5000/get',{params: params}).pipe(
      tap(async (res:  ArduinoData ) => {
        if (res) {
          console.log(res);
          return res;
        }
      })
    )
  }

  getServices(api_key: string, arduino_key:string): Observable<Services>{
    let params = new HttpParams();

    params = params.append('api_key', api_key);
    params = params.append('arduino_key', arduino_key);

    return this.httpClient.get<Services>('http://157.230.107.10:5000/get_services',{params: params}).pipe(
      tap(async (res:  Services ) => {
        if (res) {
          console.log("Services", res);
          return res;
        }
      })
    )
  }

  updateService(api_key: string, arduino_key:string, service: Service, create: Boolean = false){
    let payload = {}
    if (create == false) {
      payload = {
        api_key: api_key,
        arduino_key: arduino_key,
        service_name: service.name,
        service_id: service.id,
        start_time: service.start_time,
        end_time: service.end_time,
        active: service.active
      }
    }
    else {
      payload = {
        api_key: api_key,
        arduino_key: arduino_key,
        service_name: service.name,
        start_time: service.start_time,
        end_time: service.end_time,
        active: service.active
      }
    }
    return this.httpClient.post('http://157.230.107.10:5000/set_service', payload).pipe(
      tap(async (res: string) => {
        console.log(res);
        return res;
      })
    );
  }

  registerArduino(api_key: string, arduino_name: string): Observable<any>{
    let payload = {
      api_key: api_key,
      arduino_name: arduino_name
    }
    console.log("Adding new arduino with payload", payload)
    return this.httpClient.post<any>('http://157.230.107.10:5000/register_arduino', payload).pipe(
      tap(async (res ) => {
        console.log("Register Arduinio: ", res);

        if (res) {
          console.log("Register Arduinio: ", res);
          return res;
        }
      })
    )

  }
}

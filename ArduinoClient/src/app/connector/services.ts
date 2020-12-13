export interface Service {
    name: string;
    start_time: string;
    end_time: string;
    active: string;
    id?: number;
}

export interface Services {
    arduino: Array<Service>
}

export class ServiceClass {

    constructor(
        public name: any,
        public start_time: string,
        public end_time: string,
        public active: string,
    ) {  }
  
  } 
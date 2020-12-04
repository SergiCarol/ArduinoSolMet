export interface Service {
    name: string;
    start_time: string;
    end_time: string;
    active: string;
    id: number;
}

export interface Services {
    arduino: Array<Service>
}

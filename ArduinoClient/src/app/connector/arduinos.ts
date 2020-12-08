export interface Arduino {
    nama: string;
    id: number;
    api_key: string;
}

export interface Arduinos {
    arduino: Array<Arduino>
}

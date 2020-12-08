import { TestBed } from '@angular/core/testing';

import { ArduinoLocalService } from './arduino-local.service';

describe('ArduinoLocalService', () => {
  let service: ArduinoLocalService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ArduinoLocalService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});

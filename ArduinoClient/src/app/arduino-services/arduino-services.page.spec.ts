import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { ArduinoServicesPage } from './arduino-services.page';

describe('ArduinoServicesPage', () => {
  let component: ArduinoServicesPage;
  let fixture: ComponentFixture<ArduinoServicesPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ArduinoServicesPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(ArduinoServicesPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

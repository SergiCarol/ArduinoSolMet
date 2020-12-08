import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { ArduinoDataPage } from './arduino-data.page';

describe('ArduinoDataPage', () => {
  let component: ArduinoDataPage;
  let fixture: ComponentFixture<ArduinoDataPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ArduinoDataPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(ArduinoDataPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

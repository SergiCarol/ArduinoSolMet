import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { AddArduinoDataPage } from './add-arduino-data.page';

describe('AddArduinoDataPage', () => {
  let component: AddArduinoDataPage;
  let fixture: ComponentFixture<AddArduinoDataPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AddArduinoDataPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(AddArduinoDataPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

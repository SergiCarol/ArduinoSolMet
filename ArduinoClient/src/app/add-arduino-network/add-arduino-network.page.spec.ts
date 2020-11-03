import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { IonicModule } from '@ionic/angular';

import { AddArduinoNetworkPage } from './add-arduino-network.page';

describe('AddArduinoNetworkPage', () => {
  let component: AddArduinoNetworkPage;
  let fixture: ComponentFixture<AddArduinoNetworkPage>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AddArduinoNetworkPage ],
      imports: [IonicModule.forRoot()]
    }).compileComponents();

    fixture = TestBed.createComponent(AddArduinoNetworkPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

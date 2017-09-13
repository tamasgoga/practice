import {InMemoryDbService} from 'angular-in-memory-web-api';

export class InMemoryDataService implements InMemoryDbService {
	createDb() {
		let heroes = [
		  {id: 11, name: 'Deadpal'},
		  {id: 12, name: 'Batmoon'},
		  {id: 13, name: 'Supperman'},
		  {id: 14, name: 'Glamour Gal'},
		  {id: 15, name: 'Hider-man'},
		  {id: 16, name: 'Mr. Fantashtick'},
		  {id: 17, name: 'Brandalf'},
		  {id: 18, name: 'Brodusseus'},
		  {id: 19, name: 'Chuckilles'},
		  {id: 20, name: 'Major Canada'}
		];

		return {heroes};
	}
}

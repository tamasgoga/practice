import {Component, OnInit} from '@angular/core';
import {Router} from '@angular/router';

import {Observable} from 'rxjs/Observable';
import {Subject} from 'rxjs/Subject';

// extension to Observable
import 'rxjs/add/observable/of';

// operators for Observable
import 'rxjs/add/operator/catch';
import 'rxjs/add/operator/debounceTime';
import 'rxjs/add/operator/distinctUntilChanged';

import {HeroSearchService} from './hero-search.service';
import {Hero} from './hero';

@Component({
	moduleId: module.id,
	selector: 'hero-search',
	templateUrl: './hero-search.component.html',
	styleUrls: ['./hero-search.component.css'],
	providers: [HeroSearchService]
})
export class HeroSearchComponent implements OnInit {
	heroes: Observable<Hero[]>;
	private searchTerms = new Subject<string>();

	constructor(
		private heroSearchService: HeroSearchService,
		private router: Router
	) {}

	// push search term
	search(term: string): void {
		this.searchTerms.next(term);
	}

	ngOnInit(): void {
		this.heroes = this.searchTerms
			.debounceTime(300) // 300 ms delay after each keystroke
			.distinctUntilChanged() // ignore if the next term is the same as the previous
			.switchMap( // return the search observable, or one containing empty heroes (if search is empty)
				term => term ? this.heroSearchService.search(term) : Observable.of<Hero[]>([])
			).catch(error => {
				// real error handling would go here
				console.error(error);
				return Observable.of<Hero[]>([]);
			});
	}

	gotoDetail(hero: Hero): void {
		let link = ['/detail', hero.id];
		this.router.navigate(link);
	}
}

#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <stddef.h>
#include <vector>

namespace ShellSort
{
	template <typename Type>
	void sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		for (long gap_index = gap_sequence.size() - 1; gap_index >= 0; gap_index -= 1)
		{
			long gap = gap_sequence[gap_index];
			for (long unsorted_cursor = (before_first + 1) + gap; unsorted_cursor < after_last; unsorted_cursor += 1)
			{
				int a;
				Type temp = data[unsorted_cursor];
				long insertion_cursor;
				for (insertion_cursor = unsorted_cursor; insertion_cursor - (before_first + 1) >= gap && data[insertion_cursor - gap] > temp; insertion_cursor -= gap)
				{
					data[insertion_cursor] = data[insertion_cursor - gap];
				}
				data[insertion_cursor] = temp;
			}
		}
	}

	// If you don't want to use the operating system's stack, then this would hypothetically be a good option.
	template <typename Type>
	void lazy_sort(Type *data, long before_first, long after_last, std::vector<long> gap_sequence)
	{
		// Start with InsertionSort::lazy_leftward_sort(data, before_first, after_last, [](long n){return 2*n;});
		// If done, return
		// Otherwise lazily shell sort, the first iteration might be straightforward-ish, but the rest have substantial room for optimization (in this case, I'm actually interested in optimizing)
		// Optimization: keep track of the leftmost "first unsorted" node
		// Optimization: increase function (gap sequence number) when a threshold like 2n has been crossed.
		//    NOTE: why is this important? You don't want to sort in e.g. O(nlglgn) when the same work could be done in O(k)
		// Optimization: any time an insertion happens you can probably ignore the gap number and just count the number of insertions
		// Optimization(?): you could consider having thresholds go forwards and backwards (gap sequence number can reduce as well depending on budget)
		//    E.g. if items are sorted within a range of 64 places of their correct position and the gap sequence number of 65 continuously fails, then the number should reduce accordingly.
		//    Similarly if the gap sequence number is 5 and it often shifts 10 places (50 among the 64) then the order should be increased accordingly.
		// When you hit the end, you start from the "first unsorted" node and scan forwards with a gap sequence number of 1 again.
		//    If you ever hit the end with a gap sequence of 1, then the series is sorted.
		// A node is eligible to be a "first unsorted" node when it is inserted with a gap number other than 1 (because it could be imprecise).
		// When you increase the gap sequence number, you have a guarantee you have travelled a substantial distance, so subtracting the difference between the next gap number and the current gap number should not go out of bounds (although you should still do boundary checking because it should fail occasionally).
		// Optimization: when budget exceeds above or dwindles below it's Goldilocks zone, you should switch from one gap sequence number to another.
	}

	std::vector<long> generalized_pratt(std::set<long> factors_list, long last_value, bool exponentiate = true)
	{
		// Using ordered sets makes the implementation user-proof (sort of).
		// Duplicate and misordered entries aren't a problem, although negative numbers still are.
		// While negative numbers may first appear to be bad, I don't think it would be meaningless to augment the algorithm to account for them.
		// The only real change is the invariant where the leftmost partitioned elements never changed goes away
		// And is replaced by an outwards expansion from zero (where the "middle" elements never change).
		factors_list.insert(1);
		if (factors_list.size() == 1)
		{
			// fancy way of saying "return {1}"
			return std::vector<long>(factors_list.begin(), factors_list.end());
		}
		// The following code guards against user errors when: (user-proofing / defensive programming)
		//     1) values < factors_list.size() or
		//     2) the largest factor is greater than the nth value
		std::set<long> unexponentiated(factors_list.begin(), factors_list.end());
		for (int guaranteed_value = 0; true; guaranteed_value += 1)
		{
			// Set cannot be modified while iterators are traversing, but we can avoid that altogether.
			long current_value = *std::next(unexponentiated.begin(), guaranteed_value);
			if (current_value > last_value)
			{
				break;
			}
			for (long factor : factors_list) // this includes 1 but I don't mind
			{
				unexponentiated.insert(current_value*factor);
			}
		}

		// For e.g. a Pratt<2,3> you want to square the sequence to 4,9
		// For higher powers, I don't think it's quite the same
		// E.g. for a Pratt<2,3,5> I think you don't want numbers in the same fashion. Hence this code.
		int power = factors_list.size() - 1;
		if (!exponentiate)
		{
			power = 1;
		}
		std::vector<long> exponentiated;
		for (int index = 0; true; index += 1)
		{
			long value = *std::next(unexponentiated.begin(), index);
			long result = 1;
			// kinda equivalent to pow(value, power);
			for (int lazy_power = 0; lazy_power < power; lazy_power += 1)
			{
				result *= value;
			}
			if (result > last_value)
			{
				break;
			}
			exponentiated.push_back(result);
		}

		return exponentiated;
	}

	std::vector<long> gap_sequence_generator(std::function<long(long)> lambda_function, long values)
	{
		std::vector<long> result;
		for (int value = 1; value <= values; value += 1)
		{
			result.push_back(lambda_function(value));
		}

		return result;
	}


	const double e = 2.7182818284590452353602874713527;
	const double phi = 1.61803398874989484820;
	const double pi = 3.14159265358979323846;

	std::function<long(long)> ciura_approximation2 = [](long n)
	{
		const double r = 2.377411604869019028015819013165746863809012622871126918401; // (n+sqrt(n^2+4))/2, n=2-1/e^(pi)
		double value = 1;
		for (int iteration = 1; iteration < n; iteration += 1)
		{
			value = r * value + 1;
		}
		return ceil(value);
	};

	std::vector<long> ciura_gap_sequence = { 1, 4, 10, 23, 57, 132, 301, 701,
		// The following values are extrapolated via T(n) = floor(T(n-1)*2.25)
		1577, 3548, 7983, 17961, 40412, 90927, 204585, 460316, 1035711 };

	std::vector<long> pratt_three_smooth_gap_sequence =
	{
		1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 27, 32, 36, 48, 54, 64, 72, 81, 96, 108, 128, 144, 162, 192, 216, 243,
		256, 288, 324, 384, 432, 486, 512, 576, 648, 729, 768, 864, 972, 1024, 1152, 1296, 1458, 1536, 1728, 1944,
		2048, 2187, 2304, 2592, 2916, 3072, 3456, 3888
	};

	std::vector<long> tokuda_gap_sequence =
	{
		1, 4, 9, 20, 46, 103, 233, 525, 1182, 2660, 5985, 13467, 30301, 68178, 153401, 345152, 776591, 1747331,
		3931496, 8845866, 19903198, 44782196, 100759940, 226709866, 510097200, 1147718700
	};

	std::vector<long> sedgewick_incerpi_sequence =
	{
		1, 3, 7, 21, 48, 112, 336, 861, 1968, 4592, 13776, 33936, 86961, 198768, 463792, 1391376, 3402672, 8382192,
		21479367, 49095696, 114556624, 343669872, 852913488, 2085837936
	};

	// TODO: variable primorial gap sequence using k#/i# (thus it e.g. sorts on 2, then 3, then 5 instead of 5 then 3 then 2)
	// primorial: 1, 2, 6, 30, 210, 2310, 30030, 510510,
	// "k#/i#": 510510/1, 510510/2, 510510/6, 510510/30, 510510/210, 510510/2310, 510510/30030, 510510/510510, (reverse ordered and only an example since it's variable)
	// upon thinking about it more, I feel like this wouldn't worth but I might as well try it if I have time.
	// Extending to other values that aren't arrays of size k#: 1) sort individual subarrays and merge or 2) double over with more primorials with remainder or 3) interpolate with standard gap sequences to get to primorial sorting

	// concept:
	// First generalized_pratt<2> number
	// Next generalized_pratt<2, 3> numbers
	// Next generalized_pratt<3, 5, 7> numbers
	// Next generalized_pratt<2, 3, 5, 7> numbers, etc
	// Next generalized_pratt<2, 3, 5, 7, 11> numbers, etc
	std::vector<long> probably_reliable =
	{
		1, 2,
		4, 9, 16,
		27, 64, 125, 216, 512,
		625, 1296, 2401, 4096, 6561, 10000,
		16807, 32768, 59049, 100000, 161051, 248832, 537824, 759375, 1048576,
		1771561, 2985984, 4826809, 7529536, 11390625, 16777216, 34012224,
		62748517
	};

	std::vector<long> tetrahedral_subset = // this does terrible.
	{
		1, 4, 10, 20, 56, 120, 286, 680, 1540, 3276, 7140, 15180
	};

	std::vector<long> triangular_subset = // this does really well.
	{
		//1, 3, 6, 15, 36, 78, 171, 351, 703, 1431 // triangular numbers but ratio must be at least 2
		//1, 3, 10, 21, 45, 91, 190, 406, 903 // triangular numbers but insert gaps equal to fibonacci sequence
		//1, 3, 6, 15, 36, 91, 231, 595 // Fibonacci indices from triangular numbers

		// indices: 1, 2, 4, 6, 9, 13, 19,
		1, /**/ // 0
		3, /*6,*/ // 1
		10, /*15,*/ // 1
		21, /*28, 36,*/ // 2
		45, /*55, 66, 78,*/ // 3
		91, /*105, 120, 136, 153, 171,*/ // 5
		190, /*210, 231, 253, 276, 300, 325, 351, 378,*/ // 8
		406, /*435, 465, 496, 528, 561, 595, 630, 666, 703, 741, 780, 820, 861,*/ // 13
		903, /*946, 990, 1035, 1081, 1128, 1176, 1225, 1275, 1326, 1378, 1431, 1485, 1540, 1596, 1653, 1711, 1770, 1830, 1891, 1953, 2016,*/ // 21
		2080, /*2145, 2211, 2278, 2346, 2415, 2485, 2556, 2628, 2701, 2775, 2850, 2926, 3003, 3081, 3160, 3240, 3321, 3403, 3486, 3570, 3655, 3741, 3828, 3916, 4005, 4095, 4186, 4278, 4371, 4465, 4560, 4656, 4753, 4851,*/ // 34
		4950, /*5050, 5151, 5253, 5356, 5460, 5565, 5671, 5778, 5886, 5995, 6105, 6216, 6328, 6441, 6555, 6670, 6786, 6903, 7021, 7140, 7260, 7381, 7503, 7626, 7750, 7875, 8001, 8128, 8256, 8385, 8515, 8646, 8778, 8911, 9045, 9180, 9316, 9453, 9591, 9730, 9870, 10011, 10153, 10296, 10440, 10585, 10731, 10878, 11026, 11175, 11325, 11476, 11628, 11781, 11935,*/ // 55
		12090, //, 12246, 12403, 12561, 12720, 12880, 13041, 13203, 13366, 13530, 13695, 13861, 14028, 14196, 14365, 14535, 14706, 14878, 15051, 15225, 15400, 15576, 15753, 15931, 16110, 16290, 16471, 16653, 16836, 17020, 17205, 17391, 17578, 17766, 17955, 18145, 18336, 18528, 18721, 18915, 19110, 19306, 19503, 19701, 19900, 20100, 20301, 20503, 20706, 20910, 21115, 21321, 21528, 21736, 21945, 22155, 22366, 22578, 22791, 23005, 23220, 23436, 23653, 23871, 24090, 24310, 24531, 24753, 24976, 25200, 25425, 25651, 25878, 26106, 26335, 26565, 26796, 27028, 27261, 27495, 27730, 27966, 28203, 28441, 28680, 28920, 29161, 29403, 29646, 29890, 30135, 30381, 30628, 30876, 31125, 31375, 31626, 31878, 32131, 32385, 32640, 32896, 33153, 33411, 33670, 33930, 34191, 34453, 34716, 34980, 35245, 35511, 35778, 36046, 36315, 36585, 36856, 37128, 37401, 37675, 37950, 38226, 38503, 38781, 39060, 39340, 39621, 39903, 40186, 40470, 40755, 41041, 41328, 41616, 41905, 42195, 42486, 42778, 43071, 43365, 43660, 43956, 44253, 44551, 44850, 45150, 45451, 45753, 46056, 46360, 46665, 46971, 47278, 47586, 47895, 48205, 48516, 48828, 49141, 49455, 49770, 50086, 50403, 50721, 51040, 51360, 51681, 52003, 52326, 52650, 52975, 53301, 53628, 53956, 54285, 54615, 54946, 55278, 55611, 55945, 56280, 56616, 56953, 57291, 57630, 57970, 58311, 58653, 58996, 59340, 59685, 60031, 60378, 60726, 61075, 61425, 61776, 62128, 62481, 62835, 63190, 63546, 63903, 64261, 64620, 64980, 65341, 65703, 66066, 66430, 66795, 67161, 67528, 67896, 68265, 68635, 69006, 69378, 69751, 70125, 70500, 70876, 71253, 71631, 72010, 72390, 72771, 73153, 73536, 73920, 74305, 74691, 75078, 75466, 75855, 76245, 76636, 77028, 77421, 77815, 78210, 78606, 79003, 79401, 79800, 80200, 80601, 81003, 81406, 81810, 82215, 82621, 83028, 83436, 83845, 84255, 84666, 85078, 85491, 85905, 86320, 86736, 87153, 87571, 87990, 88410, 88831, 89253, 89676, 90100, 90525, 90951, 91378, 91806, 92235, 92665, 93096, 93528, 93961, 94395, 94830, 95266, 95703, 96141, 96580, 97020, 97461, 97903, 98346, 98790, 99235, 99681, 100128, 100576, 101025, 101475, 101926, 102378, 102831, 103285, 103740, 104196, 104653, 105111, 105570, 106030, 106491, 106953, 107416, 107880, 108345, 108811, 109278, 109746, 110215, 110685, 111156, 111628, 112101, 112575, 113050, 113526, 114003, 114481, 114960, 115440, 115921, 116403, 116886, 117370, 117855, 118341, 118828, 119316, 119805, 120295, 120786, 121278, 121771, 122265, 122760, 123256, 123753, 124251, 124750, 125250, 125751, 126253, 126756, 127260, 127765, 128271, 128778, 129286, 129795, 130305, 130816, 131328, 131841, 132355, 132870, 133386, 133903, 134421, 134940, 135460, 135981, 136503, 137026, 137550, 138075, 138601, 139128, 139656, 140185, 140715, 141246, 141778, 142311, 142845, 143380, 143916, 144453, 144991, 145530, 146070, 146611, 147153, 147696, 148240, 148785, 149331, 149878, 150426, 150975, 151525, 152076, 152628, 153181, 153735, 154290, 154846, 155403, 155961, 156520, 157080, 157641, 158203, 158766, 159330, 159895, 160461, 161028, 161596, 162165, 162735, 163306, 163878, 164451, 165025, 165600, 166176, 166753, 167331, 167910, 168490, 169071, 169653, 170236, 170820, 171405, 171991, 172578, 173166, 173755, 174345, 174936, 175528, 176121, 176715, 177310, 177906, 178503, 179101, 179700, 180300, 180901, 181503, 182106, 182710, 183315, 183921, 184528, 185136, 185745, 186355, 186966, 187578, 188191, 188805, 189420, 190036, 190653, 191271, 191890, 192510, 193131, 193753, 194376, 195000, 195625, 196251, 196878, 197506, 198135, 198765, 199396, 200028, 200661, 201295, 201930, 202566, 203203, 203841, 204480, 205120, 205761, 206403, 207046, 207690, 208335, 208981, 209628, 210276, 210925, 211575, 212226, 212878, 213531, 214185, 214840, 215496, 216153, 216811, 217470, 218130, 218791, 219453, 220116, 220780, 221445, 222111, 222778, 223446, 224115, 224785, 225456, 226128, 226801, 227475, 228150, 228826, 229503, 230181, 230860, 231540, 232221, 232903, 233586, 234270, 234955, 235641, 236328, 237016, 237705, 238395, 239086, 239778, 240471, 241165, 241860, 242556, 243253, 243951, 244650, 245350, 246051, 246753, 247456, 248160, 248865, 249571, 250278, 250986, 251695, 252405, 253116, 253828, 254541, 255255, 255970, 256686, 257403, 258121, 258840, 259560, 260281, 261003, 261726, 262450, 263175, 263901, 264628, 265356, 266085, 266815, 267546, 268278, 269011, 269745, 270480, 271216, 271953, 272691, 273430, 274170, 274911, 275653, 276396, 277140, 277885, 278631, 279378, 280126, 280875, 281625, 282376, 283128, 283881, 284635, 285390, 286146, 286903, 287661, 288420, 289180, 289941, 290703, 291466, 292230, 292995, 293761, 294528, 295296, 296065, 296835, 297606, 298378, 299151, 299925, 300700, 301476, 302253, 303031, 303810, 304590, 305371, 306153, 306936, 307720, 308505, 309291, 310078, 310866, 311655, 312445, 313236, 314028, 314821, 315615, 316410, 317206, 318003, 318801, 319600, 320400, 321201, 322003, 322806, 323610, 324415, 325221, 326028, 326836, 327645, 328455, 329266, 330078, 330891, 331705, 332520, 333336, 334153, 334971, 335790, 336610, 337431, 338253, 339076, 339900, 340725, 341551, 342378, 343206, 344035, 344865, 345696, 346528, 347361, 348195, 349030, 349866, 350703, 351541, 352380, 353220, 354061, 354903, 355746, 356590, 357435, 358281, 359128, 359976, 360825, 361675, 362526, 363378, 364231, 365085, 365940, 366796, 367653, 368511, 369370, 370230, 371091, 371953, 372816, 373680, 374545, 375411, 376278, 377146, 378015, 378885, 379756, 380628, 381501, 382375, 383250, 384126, 385003, 385881, 386760, 387640, 388521, 389403, 390286, 391170, 392055, 392941, 393828, 394716, 395605, 396495, 397386, 398278, 399171, 400065, 400960, 401856, 402753, 403651, 404550, 405450, 406351, 407253, 408156, 409060, 409965, 410871, 411778, 412686, 413595, 414505, 415416, 416328, 417241, 418155, 419070, 419986, 420903, 421821, 422740, 423660, 424581, 425503, 426426, 427350, 428275, 429201, 430128, 431056, 431985, 432915, 433846, 434778, 435711, 436645, 437580, 438516, 439453, 440391, 441330, 442270, 443211, 444153, 445096, 446040, 446985, 447931, 448878, 449826, 450775, 451725, 452676, 453628, 454581, 455535, 456490, 457446, 458403, 459361, 460320, 461280, 462241, 463203, 464166, 465130, 466095, 467061, 468028, 468996, 469965, 470935, 471906, 472878, 473851, 474825, 475800, 476776, 477753, 478731, 479710, 480690, 481671, 482653, 483636, 484620, 485605, 486591, 487578, 488566, 489555, 490545, 491536, 492528, 493521, 494515, 495510, 496506, 497503, 498501, 499500, 500500,
		30135, 76245, 195000, 502503, 1301691, 3383901, 8817900, 23014720, 60132061, 157220778, 411256860
	};

	std::vector<long> triangular_subset2 =
	{
		// indices: ceil((1.5)^(k-1))
		1, 3, 6, 10, 21, 36, 78, 171, 351, 780,
		1711, 3828, 8515, 19110, 42778, 96141, 216153, 486591, 1092981, 2458653,
		5532801, 12442566, 27993903, 62983476, 141717030, 32663181
	};

	std::vector<long> triangular_subset3 =
	{
		1, 4, 10, 23, 57, 132, 301, 701,

		1378, 3321, 7875, 18721, 44850, 107416, 257403, 616605, 1481781, 3557778, 8547045, 20528028, 49317346, 118479921, 32887612
	};
}
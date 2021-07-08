#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>

using namespace std;
class PositiveReview {
public:
	int businessId;
	int userId;
	PositiveReview(int businessId, int userId) : businessId(businessId), userId(userId) {};

};
using ps = PositiveReview;

//two business are similar if their intersection of reviews done by user / union of their users is close to 1. A intersection B / A union B
//this solution is O(N) space, O(NLogN) because using map (LogN for insertion, keeping an ordered tree)
//transform the positivereview vector into a map which links businessid with their set of unique users review. find the intersections and unions between each businessid and the interseted one. store the most similar, return it
int find_most_similar_business(int businessOfInterestId, const vector<PositiveReview>& positiveReviews) {
	//create one set for each businessid
	map<int, set<int>> businessUsersMap;//key:businessid, value: set<userid>
	for (PositiveReview review : positiveReviews) {
		auto it = businessUsersMap.find(review.businessId);
		if (it == businessUsersMap.end()) {//business is in the map
			businessUsersMap.insert(make_pair(review.businessId, set<int> {review.userId}));
		}
		else {//business is already present
			//add the user to the set
			it->second.insert(review.userId);
		}
	}
	auto itBusinessInterest = businessUsersMap.find(businessOfInterestId);
	//debug
	assert(itBusinessInterest != businessUsersMap.end());
	//all sets are created. find intersections and unions between set
	double maxsimilarity = 0;
	int maxsimilarbusiness = businessOfInterestId;//just initalize
	for (auto businessPair : businessUsersMap) {
		if (businessPair.first == businessOfInterestId) continue;
		vector<int> intersection, unions;
		//find intersections and unions using stl
		set_intersection(itBusinessInterest->second.begin(), itBusinessInterest->second.end(), businessPair.second.begin(), businessPair.second.end(), back_inserter(intersection));
		set_union(itBusinessInterest->second.begin(), itBusinessInterest->second.end(), businessPair.second.begin(), businessPair.second.end(), back_inserter(unions));
		double similarity = static_cast<double>(intersection.size()) / unions.size();//have to cast at least 1 element to double, so double division will be used. otherwise int is returned and then stored into double
		if (similarity > maxsimilarity) {
			maxsimilarity = similarity;
			maxsimilarbusiness = businessPair.first;
		}
	}
	return maxsimilarbusiness;
}


int main() {
	vector<ps> reviews{ ps(10, 1), ps(10, 2), ps(10, 3), ps(9, 5), ps(8, 2), ps(8, 3) };
	cout << "most similar business is " + find_most_similar_business(10, reviews);

	return 0;
}
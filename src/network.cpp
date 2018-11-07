#include "network.h"
#include "random.h"

/*!  Resizes the list of nodes (\ref values) and also resets all values.
    After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
*/
void Network::resize(const size_t& s){
	values.clear();
	for (size_t i=0; i < s; ++i){
		values.push_back(RNG.normal(0, 1));	//Pour toutes les cases du tableau, on fait un poush back avec une valeur distribuee normalement
	}	
}
	
/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted (true if both nodes exist and the link did not exist yet)
 */
bool Network::add_link(const size_t& a, const size_t& b){
	if (a >= values.size() or b >= values.size() or a == b){	//verifie si a et b existent ou sont egaux
		return false;
	}	
	for(auto i : links){
		if(i.first == a and i.second == b){
			return false;
		} 
	}
		links.insert({a,b});
		links.insert({b,a});
		return true;
}
	
/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen),
  where *degree(n)* is Poisson-distributed.

  All previous links are erased first.
  @param[in] mean_deg the average of the Poisson distribution,
  @param[out] number of links created.
 */
size_t Network::random_connect(const double& m){
	links.clear();
	for(size_t i = 0; i < values.size(); i++){
		int degree_n (RNG.poisson(m));
		std::vector<int> nodes(degree_n - 1);
		for(auto n : nodes){
			if(add_link(i, n)){}
		}		
	}
	return links.size()/2;
}
	
/*! Resets node values with a vector of *n* new values.
  If the current size is *s* and *n<s* then only the first *n* values are changed.
  If *n>s* then only *s* values are used. 
  The internal \ref values vector keeps the same size.

  @param[in] vector of new node values,
  @param[out] number of nodes succesfully reset.
 */
size_t Network::set_values(const std::vector<double>& _v){
		if(_v.size() < values.size()){
			for (size_t i = 0; i < _v.size(); ++i){
				values[i] = _v[i];
			}
			return _v.size();
		} else if (_v.size() >= values.size()){
			for (size_t i = 0; i < values.size(); ++i){
				values[i] = _v[i];
			}
			return values.size();
		}
	
}
	
/*! Number of nodes */
size_t Network::size() const{
	return values.size();
}
	
/*! Degree (number of links) of node no *n* */
size_t Network::degree(const size_t &_n) const{
	return links.count(_n);
}
	
/*! Value of node no *n* */
double Network::value(const size_t &_n) const{
	return values[_n];
}
	
/*! All node values in descending order
  \ref values is left unchanged by this operation. */
std::vector<double> Network::sorted_values() const{
	std::vector<double> _values(values);
	std::sort(_values.begin(), _values.end());
	std::reverse(_values.begin(), _values.end());
	return _values;
}
	
/*! All neighbors (linked) nodes of node no *n* */
std::vector<size_t> Network::neighbors(const size_t &_n) const{
	std::vector<size_t> _neighbors(0);
			auto it = links.equal_range(_n);
			for (auto itr = it.first; itr != it.second; ++itr) { 
				_neighbors.push_back(itr->second);
			}
	return _neighbors;
}

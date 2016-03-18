<?php
$dir = dirname(__FILE__);
require_once('TbsApi.inc');
require_once('TbsCustomer.inc');
require_once('TbsObject.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 1/15/2016
* 
*/

class TbsLocation extends TbsObject
{
	private $TbsApi;

	function __construct(){
		// maybe this should use dependancy injection?
		$this->TbsApi = TbsApi::getInstance();
	}

	/*
	* retrieve a collection of location objects
	*
	* Params
	* $howMany : the number of records to return
	* 
	* Returns : an array of object upon success, false upon failure
	*
	* TODO: the objects returned should be Tbs location not generic
	*/
	function getCollection($howMany){
		return $this->TbsApi->getResource('locations', null, $howMany);
	}

	/*
	* retrieve a specific location object
	*
	* Params
	* $locationsKey : the unique key for the location
	*
	* returns : $this, with the retrieved location data applied, false upon failure
	*/
	function get($locationsKey){
		$result = $this->TbsApi->getResource('locations', $locationsKey);
		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	function getDefaults(){
		$locationDefaults = $this->TbsApi->getresource('locations/defaults', null, null);
		if($locationDefaults){
			return $this->applyData($locationDefaults);
		} else {
			return false;
		}
	}

	/* 
	* Creates a new customer from the data set on this object for the given customer key
	* 
	* params : 
	* $customersKey : the key for the customer to which this location will be associated
	* 
	* return: the created object returned from the remote server, false upon failure
	*
	*/
    function create($customersKey){
    	// get location defaults
    	//$defaults = $this->getDefaults();

    	// get a unique code per this customer
    	$tbsCustomer = new TbsCustomer();

    	// set the customers key on this object
		$this->CustomersKey = $customersKey;

    	// set the location code on $this
    	$this->LocationCode = $tbsCustomer->getAvailableLocationCode($customersKey);

		// make the call
		$result = $this->TbsApi->post('locations', $this);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
    }

	/*
	* updates a location record with the data set on this object
	*
	* return : object with data returned from remote server, false upon failure
	*
	* notes : BE AWARE THAT THERE IS BAD DATA IN THE DB. REPOSTING IT WILL CAUSE A MYSTERIOUS 500 ERROR
	*/ 
	function update(){
		$result = $this->TbsApi->put('locations' . '/' . $this->LocationsKey, $this);

		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	/*
	* return the unique identifier for this object
	*
	* return : int, false upon failure
	*/
	function getLocationKey(){
		if($this->LocationsKey){
			return $this->LocationsKey;
		} else {
			return false;
		}

	}
}

?>
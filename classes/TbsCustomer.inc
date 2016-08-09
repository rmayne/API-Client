<?php
$dir = dirname(__FILE__);
require_once('TbsApi.inc');
require_once('TbsObject.inc');

/*
* Author: github.com/rmayne
* Date: 12-23-2015
*Parent class functions:
todo: location defaults is create customer should be a location method
remove the error handling???
update() should return this, or false

*/

class TbsCustomer extends TbsObject
{
	private $TbsApi;
	private $location;

	function __construct(){
		$this->TbsApi = TbsApi::getInstance();
	}

	/*
	* retrieve a collection of customer objects
	*
	* Params
	* $howMany : the number of records to return
	* 
	* Returns : an array of object upon success, false upon failure
	*
	* TODO: the bjects returned should be Tbs Customer not generic
	*/
	function getCollection($howMany){
		return $this->TbsApi->getResource('customers', null, $howMany);
	}

	/*
	* retrieve a specific customer object
	*
	* Params
	* $customersKey : the unique key for the customer
	*
	* returns : $this, with the retrieved customer data applied, false upon failure
	*/
	function get($customersKey){
		$result = $this->TbsApi->getResource('customers', $customersKey);
		if($result){
			return $this->applyData($result);
		} else {
			return false;
		}
	}

	/*
	* gets the default customer values
	* return: object representation of default customer with default data applied, false upon failure
	*
	* Notes:
	* currently this returns the defaults for the first base.
	* it will need to be modified to allow the selection of bases in the future
	* 
	*/
    function getDefaults(){
    	$baseId = $this->TbsApi->getBasesList()[0]->LkBaseInfoKey; //Arbitrarily grabs the first base ID available
    	$resourceUrl = "bases/$baseId/CustomerAddDefaults";
		return $this->TbsApi->getResource($resourceUrl);
    }

	/*
	*  check for a duplicate Customer
	* Params:
	* $name : the name field will be searched for this inorder to find a match
	* returns : an array of duplicate objects, false upon failure
	*
	* TODO : name should not be passed, it should be applied to tis object before calling
	*/
	function checkForDuplicate($name){
		$filters = array(
			array(
				"field" => 'Name',
				"filter" => 'Contains',
				"value" => $name
			)
		);
		return $this->TbsApi->getResource('customers', null, 'All', $filters, null);
	}

	/*
	* sets the location to create a customer
	*
	* Params : 
	* $location : a location object that will be used to create a new customer
	*
	* returns : $this object
	*/
	function setLocation($location){
		$this->location = $location;
		return $this;
	}

	/* 
	* Creates a new customer from the data set on this object
	* 
	* return: the created object returned from the remote server, false upon failure
	*
	* todo : CLEAN THIS UP
	*
	* Notes: it is allowed to specify customer # I believe 
	*/
    function create(){
    	//Get default customer defaults, and assign values to required properties
		//$customerDefaults = $this->getCustomerDefaults();
		// get location Defaults, and assign values to required properties
		//$this->location = $this->TbsApi->getresource('locations/defaults', null, null);
		$genericObj = new stdClass();
		$genericObj->Customer = $this;
		$genericObj->BillToLocation = $this->location;
		// makes the API call, applies the returned data to this object and returns itself
		$result = $this->TbsApi->post('customers', $genericObj);
		if($result){
			return $this->applyData($result->Customer);
		} else {
			return false;
		}
    }

	/*
	* return the unique identifier for this object
	*
	* return : int, false upon failure
	*/
	function getCustomerKey(){
		if($this->CustomersKey){
			return $this->CustomersKey;
		} else {
			return false;
		}

	}

	/*
	* updates a customer record with the data set on this object
	*
	* return : object returned by remote server, false upon failure
	*
	* notes : BE AWARE THAT THERE IS BAD DATA IN THE DB. REPOSTING IT WILL CAUSE A MYSTERIOUS 500 ERROR
	*/ 
	function update(){
		return $this->TbsApi->put('customers' . '/' . $this->CustomersKey, $this);
	}

	/* generates an unused location code
	* for a given customer key
	* Params:
	* $customersKey : the customer to generate a unique code for
	*
	* return : an int
	*/
	function getAvailableLocationCode($customersKey){
		// get location for this customer
		$locationCollection = $this->TbsApi->getResource("customers/$customersKey/locations", null, 'All');
		$locationCodes = array();

		// extract the codes from the locations
		foreach ($locationCollection as $location) {
			$locationCodes[] = $location->LocationCode;
		}
		
		// generate a random key and check it against the existing keys
		do {
			$unusedLocationKey = rand(1, 9999);
		} while (in_array($unusedLocationKey, $locationCodes));

		return $unusedLocationKey;
	}

	
}
?>
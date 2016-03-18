<?php
require_once('../include/TbsLocation.inc');
require_once('../include/TbsCustomer.inc');

class TbsLocationTest extends PHPUnit_Framework_TestCase {

	function testGetlocationCollection(){
		$tbsLocation = new TbsLocation();
		$locationCollection = $tbsLocation->getCollection(100);
		// select a random location from collection
		$location = $locationCollection[rand(0, 99)];
		$this->assertObjectHasAttribute('LocationsKey', $location, 'Could not get location collection');
		return $locationCollection;
	}
	
	/**
	* @depends testGetlocationCollection
	*/
	function testGetLocation($locationCollection){
		//retrun the key for a random location
		$locationKey = $locationCollection[rand(0, 99)]->LocationsKey;
		$tbsLocation = new TbsLocation();
		// get a location 
		$tbsLocation->get($locationKey);
		$this->assertObjectHasAttribute('LocationsKey', $tbsLocation, 'Could not get location');
		return $tbslocation;
	}

	function testGetDefaults(){
		$tbsLocation = new TbsLocation();
		$tbsLocation->getDefaults();
		$this->assertObjectHasAttribute('AddressClass', $tbsLocation, 'Could not get location defaults');
	}

	function testCreate(){
		$tbsCustomer = new TbsCustomer();
		//get collection of customers
		$customerCollection = $tbsCustomer->getCollection(100);
		// select a random customer key from collection
		$customerKey = $customerCollection[rand(0, 99)]->CustomersKey;

		$tbsLocation = new TbsLocation();

		// get and apply default values
		$tbsLocation->getDefaults();

		// set required values
	    $tbsLocation->City = "String content";
	    $tbsLocation->CountryCode = 840;
	    $tbsLocation->CustomersKey = 986;
	    $tbsLocation->Description = "String content";
	    $tbsLocation->E911ForceCSZ = true;
	    $tbsLocation->LocationCode = 7786;
	    $tbsLocation->State = "String content";
	    $tbsLocation->ZipCode = "26555";

		$this->assertObjectHasAttribute('LocationCode', $tbsLocation->create($customerKey), 
			'Could not create a location for customer key: ' . $customerKey
		);
	}

	function testUpdate(){
		$tbsLocation = new TbsLocation();
		$locationCollection = $tbsLocation->getCollection(100);

		// use a random location from the location
		$tbsLocation->applyData($locationCollection[rand(0, 99)]);

		// repost same data
		$result = $tbsLocation->update();

		// test return value for a location key
		$this->assertNotNull($result->LocationsKey, 'Could not update location Key: ' . $result->LocationsKey);

		return $result;
	}

	function testGetLocationKey(){
		$tbsLocation = new TbsLocation();
		$locationCollection = $tbsLocation->getCollection(100);

		// get the key from arandom location
		$locationKey = $locationCollection[rand(0, 99)]->LocationsKey;
		$this->assertNotNull($locationKey, 'Failed to retrieve location key');
	}

	// future tests
	//function testUpdateServiceDates(){}
}

?>
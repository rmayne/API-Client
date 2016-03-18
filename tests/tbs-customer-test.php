<?php
require_once('../include/TbsLocation.inc');
require_once('../include/TbsCustomer.inc');

class TbsCustomerTest extends PHPUnit_Framework_TestCase {
	
	function testGetCustomerCollection(){
		$tbsCustomer = new TbsCustomer();
		$customerCollection = $tbsCustomer->getCollection(10);
		// select a random customer from collection
		$customer = $customerCollection[rand(0, 9)];
		$this->assertObjectHasAttribute('AccountNumber', $customer, 'Could not get customer collection');
		return $customerCollection;
	}

	function testGetCustomer(){
		$tbsCustomer = new TbsCustomer();
		// get a customer w a random key 
		$tbsCustomer->get(rand(100, 200));
		$this->assertObjectHasAttribute('AccountNumber', $tbsCustomer, 'Could not get customer');
		return $tbsCustomer;
	}

	function testGetDefaults(){
		$tbsCustomer = new TbsCustomer();
		$tbsCustomer->getDefaults();
		$this->assertObjectHasAttribute('AccountNumber', $tbsCustomer, 'Could not get customer defaults');
		return $tbsCustomer;
	}

	/**
	* @depends testGetCustomerCollection
	* todo : this could return multiple potential matches, and it should not test for equality. 
	* it should test for ?contains?
	*/
	function testCheckForDuplicate(){
		$customer = new TbsCustomer();
		// get a collection of customers
		$collection = $customer->getCollection(10);
		// generate a ranodm number
		$randomIndex = rand(0,9);
		// check the equality of a random customers name against the one pulled from the DB as a duplicate
		// be aware that this expects to find only one match
		$this->assertEquals($customer->checkForDuplicate($collection[$randomIndex]->Name)[0]->Name, 
			$collection[$randomIndex]->Name,  
			'Could not retrieve a known duplicate');

		$bogusName = (string) rand(666, 6666);
		// check for a duplicate of $bogusName
		$this->assertNull($customer->checkForDuplicate($bogusName)[0], 'Returned result for known non duplicate with key: ' . $bogusName);
	}
	
	//function testSetLocation(){}

	/**
	* @depends testGetDefaults
	*/
	function testCreate($customerDefaults){
		$customer = $customerDefaults;
		// location object
		$location = new TbsLocation();
		$location->getDefaults();

		$location->AddressLine1 = "611 East Park Ave";
		$location->City = "Fairmont";
		$location->ContactName = "Snoop Dog";
		$location->CountryCode = 840;
		$location->CustomersKey = 0;
		$location->Description = "Main Location";
		$location->E911ForceCSZ = false;
		$location->InCity = false;
		$location->LocationCode = 0;
		$location->LocationsKey = 0;
		$location->MainPhone = "3048163044";
		$location->Name = "Precision Test";
		$location->State = "WV";
		$location->Type = "BillTo";
		$location->ZipCode = "26554";

		// customer bill options object
		$billOptions = new stdClass();

		$billOptions->AdjustDueDateDays = 0;
		$billOptions->AllowPaymentsBeforePaymentRuleMinimumNumberOfDays = false;
		$billOptions->AttachInvoiceToBillingEmail = false;
		$billOptions->BillOptionsKey = 0;
		$billOptions->CallFile = false;
		$billOptions->Copies = 1;
		$billOptions->CustomersKey = 0;
		$billOptions->Cycle = "OSN01";
		$billOptions->Decimals = 2;
		$billOptions->DetailSeparation = false;
		$billOptions->EmailPDF = 2;
		$billOptions->ExemptCity = false;
		$billOptions->ExemptCounty = false;
		$billOptions->ExemptFed = false;
		$billOptions->ExemptState = false;
		$billOptions->ExemptUSF = false;
		$billOptions->FinanceChargeCalcType = 1;
		$billOptions->FinanceChargePercent = 1.5;
		$billOptions->Group = "ENT";
		$billOptions->LSO = "304816";
		$billOptions->MakePDF = 0;
		$billOptions->MessageGroup = "WLCM";
		$billOptions->PreventPaymentRuleSurcharge = false;
		$billOptions->ProduceBill = 1;
		$billOptions->RoundUp = true;
		$billOptions->RptANI = "N";
		$billOptions->RptAccountCode = "N";
		$billOptions->RptCallDetail = "N";
		$billOptions->RptDayOfMonth = "N";
		$billOptions->RptInvoiceCategorySummary = "N";
		$billOptions->RptLocation = "S";
		$billOptions->RptMileage = "N";
		$billOptions->RptProductSummary = "Y";
		$billOptions->RptState = "N";
		$billOptions->RptSummaryOfCharges = "Y";
		$billOptions->RptTimeOfDay = "N";
		$billOptions->SkipLetters = false;
		$billOptions->StartFinanceCharge = 1;
		$billOptions->StorageFeeType = 0;
		$billOptions->UnlimitedLDMinutes = false;
		$billOptions->_8xxStorageFee = 0;


		$customer->setLocation($location);
		$customer->BillOptions = $billOptions;

		$customer->Balance = 0;
		$customer->BillingType = "Standard";
		$customer->CCRecurring = 0;
		$customer->Commercial = false;
		//$customer->CustomFields = ;
		$customer->CustomersKey = 0;
		$customer->DeletedFlag = false;
		$customer->Modified = false;
		$customer->Status = "New";

		$customer->create();

		$this->assertNotNull($customer->AccountNumber, 'Could not create customer');

		return $customer;
	}

	/**
	* @depends testGetCustomer
	*/
	function testUpdate(){
		$customer = new TbsCustomer();
		// generate random customer key
		$customersKey = rand(100, 200);

		// get data
		$customer->get($customersKey);

		// repost same data
		$result = $customer->update();

		// test return value for an account number
		$this->assertNotNull($result->AccountNumber, 'Could not update customer Key: ' . $customersKey);

		return $customer;
	}

	function testGetAvailableLocationCode(){
		$customer = new TbsCustomer();

		//generate random key
		$randomKey = rand(100, 200);

		$uniqueLocationCode = $customer->getAvailableLocationCode($randomKey);

		// get locations for this customer
		$TbsApi = TbsApi::getInstance();
		$locationCollection = $TbsApi->getResource("customers/$randomKey/locations", null, 'All');
		$locationCodes = array();

		// extract the codes from the locations
		foreach ($locationCollection as $location) {
			$locationCodes[] = $location->LocationCode;
		}

		$this->assertFalse(in_array($uniqueLocationCode, $locationCodes), 
			'Could not generate a unique location code for customer key: ' . $randomKey
		);
	}

	//future tests
	//function testGetCustomerKey(){}

}

?>
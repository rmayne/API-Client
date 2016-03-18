<?php
require_once('../include/TbsDetail.inc');

class TbsProductDetailTest extends PHPUnit_Framework_TestCase {

	function testGetCollection(){
		$tbsDetail = new TbsDetail();
		$detailCollection = $tbsDetail->getCollection(100);
		// select a random detail from collection
		$detail = $detailCollection[rand(0, 99)];
		$this->assertObjectHasAttribute('DetailsKey', $detail, 'Could not get detail collection');
		return $detailCollection;
	}

	//test get
	//test get defaults

	function testCreate(){
		$detail = new TbsDetail();
		$detail->getDefaults();

		$detail->ProductsKey = 6;// required, int, DB key product the charge detail is applied to
		$detail->LkDetailsKey  = 2; // required, int, DB key pointer to the charge detail definition

		$result = $detail->create();
		$this->assertObjectHasAttribute('EnteredDate', $result, 'Could not create detail');
	}

	// TODO: the data is bogus and will not validate for reposting in alot of the records
	//I have hard coded a record with known good data for the time being
	function testUpdate(){
		$tbsDetail = new TbsDetail();
		//$detailCollection = $tbsdetail->getCollection(100);

		// use a random detail from the detail
		//$tbsdetail->applyData($detailCollection[rand(0, 99)]);
		$tbsDetail->get(666); // this has know good data
		//$tbsDetail->update();
		// repost same data
		$result = $tbsDetail->update();

		// test return value for a detail key
		$this->assertNotNull($result->DetailsKey, 'Could not update detail Key: ' . $tbsdetail->DetailsKey);

		return $result;
	}

	//future tests
	//function testGetDetailKey(){}

	//function testAssignProductDetail(){}

	//function testUpdateServiceDates(){}
	
}


?>
<?php
require_once('../include/TbsProduct.inc');

class TbsProductTest extends PHPUnit_Framework_TestCase{
	
	function testGetCollection(){
		$tbsProduct = new TbsProduct();
		$productCollection = $tbsProduct->getCollection(100);
		// select a random product from collection
		$product = $productCollection[rand(0, 99)];
		$this->assertObjectHasAttribute('ProductsKey', $product, 'Could not get product collection');
		return $productCollection;
	}

	//function testGet(){}

	function testGetDefaults(){
//test returned value for a preprty
	}

	//function testGetProductForms(){}


	function testCreate(){

		$tbsProduct = new TbsProduct();
		// arbitrary product type
		$ProductFormsKey = 5;
		// get defaults
		$tbsProduct->getDefaults($ProductFormsKey);

		// add required data
		// set the minimum properties common to all product types
		$tbsProduct->CustomersKey = 945; //use int
		$tbsProduct->LocationsKey = 6002; //use int, MUST EXIST DB
		$tbsProduct->ProductID  = (string) rand(1, 1000000); // must be unique
		$tbsProduct->ProductFormsKey = $ProductFormsKey;
		$tbsProduct->ProductsKey =  0;
	    $tbsProduct->Status = "Pending";
		/* minimum for "ProductsAux_Voice" product type 
		* Your Precision Account Manager can help you determine
		* the proper values for each Product Type based on your 
		* configuration.
		* note that the product type is an object
		*/
		$tbsProduct->ProductsAux_Voice = new stdClass();
		$tbsProduct->ProductsAux_Voice->BTN = '5125551212';
		$tbsProduct->ProductsAux_Voice->LkLineTypesKey = 1; //use int. key must exist in DB
		$tbsProduct->ProductsAux_Voice->LkServiceTypesKey = 19; //use int, key must exist in DB
		$tbsProduct->ProductsAux_Voice->E911ServTypeExtensionsCount = 1; //use int, cannot be null for this product type
		$tbsProduct->ProductsAux_Voice->ProductsAux_VoiceKey = 0;
	    $tbsProduct->ProductsAux_Voice->ProductsKey = 0;

		//make the API call
		$tbsProduct->create();

		$this->assertObjectHasAttribute('EnteredDate', $tbsProduct, 'Could not create product');
		return $tbsProduct;
	}

	// TODO: the data is bogus and will not validate for reposting in alot of the records
	//I have hard coded a record with known good data for the time being
	function testUpdate(){
		$tbsProduct = new TbsProduct();
		//$productCollection = $tbsProduct->getCollection(100);

		// use a random product from the product
		//$tbsProduct->applyData($productCollection[rand(0, 99)]);
		$tbsProduct->get(666); // this has know good data
		//$tbsProduct->update();
		// repost same data
		$result = $tbsProduct->update();

		// test return value for a product key
		$this->assertNotNull($result->ProductsKey, 'Could not update product Key: ' . $tbsProduct->ProductsKey);

		return $result;
	}

	function testCheckForDuplicate(){
		$tbsProduct = new TbsProduct();
		// get a collection of products
		$collection = $tbsProduct->getCollection(100);
		// generate a ranodm number
		$randomIndex = rand(0,99);
		// check the equality of a random products name against the one pulled from the DB as a duplicate
		// be aware that this expects to find only one match
		$this->assertEquals($tbsProduct->checkForDuplicate($collection[$randomIndex]->ProductID)[0]->ProductID, 
			$collection[$randomIndex]->ProductID,  
			'Could not retrieve a known duplicate');

		$bogusProductID = 'Ax' . ((string) rand(666, 6666)) . 'FF';
		// check for a duplicate of $bogusName
		$this->assertNull($tbsProduct->checkForDuplicate($bogusProductID)[0], 'Returned result for known non duplicate with key: ' . $bogusProductID);
	}

	// future tests
	//function testGetProductsKey(){}

	//function testAssignProductDetail(){}

	//function testUpdateServiceDates(){}
	
}

?>
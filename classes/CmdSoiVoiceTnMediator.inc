<?php
require_once('../include/TbsProduct.inc');
require_once('../include/TbsApi.inc');

/*
* Author: Reiser Mayne , github.com/rmayne
* Date: 4/12/2016
* TODO: 
*/

abstract class CmdSoiVoiceTnMediator
{

	function toTbsChildProduct(ServiceOrderItemVoiceTn $voiceTn, TbsProduct $tbsProduct){

		$id = $voiceTn->getServiceorderitemid();
		$sql = "SELECT 
				soi.*,
				cba.tbs_account_number,
				cs.tbs_locations_key
			FROM service_order_item soi
			JOIN service_order so
				ON so.id = soi.service_order_id 
			JOIN client_billing_account cba 
				ON cba.id = so.client_ban
			JOIN client_site cs
				ON cs.id = so.a_site_id
			WHERE soi.id = $id
			LIMIT 1";

		// extract fileds from sql result
		$sqlResult = sqlExecute($sql);
		$tbsAccountNumber = $sqlResult->Fields('tbs_account_number');
		$tbsApi = TbsApi::getInstance();
		$tbsCustomersKey = $tbsApi->getCustomersKeyFromAccountNumber($tbsAccountNumber);
		$tbsLocationsKey = $sqlResult->Fields('tbs_locations_key');
		$itemId = $sqlResult->Fields('itemid');
		$status = $sqlResult->Fields('status');
		$parentProductsKey = $sqlResult->Fields('tbs_products_key');

		// set the product properties
		$tbsProduct->CustomersKey = (int) $tbsCustomersKey; //use int
		$tbsProduct->LocationsKey = (int) $tbsLocationsKey; //use int, MUST EXIST DB
		$tbsProduct->ProductID  = rand(1, 1000000000); //$voiceTn->getTelephonenumber();// be unique, appears to be a Alpha sku, or description
		$tbsProduct->ProductFormsKey = 5; // this is the "VOIP DID" product form/type
		$tbsProduct->ParentProductsKey = $parentProductsKey; // this is the "VOIP DID" product form/type
		//this is set by the api i think		$tbsProduct->ProductsKey =  0;
		$tbsProduct->Status = "Pending";//($status) ? $status : "Pending"; //"Pending" definitly works with the API : Active, Disconnected


		/* minimum for "ProductsAux_Voice" product type 
		* Your Precision Account Manager can help you determine
		* the proper values for each Product Type based on your 
		* configuration.
		* note that the product type is an object
		*/

		//not needed, as this object structure will be returned when the defaults are called		$tbsProduct->ProductsAux_Voice = new stdClass();
		$tbsProduct->ProductsAux_Voice->BTN = '5125551212';
		$tbsProduct->ProductsAux_Voice->LkLineTypesKey = 1; //this is the "DID" type
		$tbsProduct->ProductsAux_Voice->LkServiceTypesKey = 19; //this is for "VOIP Long Distance"
		$tbsProduct->ProductsAux_Voice->E911ServTypeExtensionsCount = 1; //use int, cannot be null for this product type
		//$tbsProduct->ProductsAux_Voice->ProductsAux_VoiceKey = 0; // appears to be assigned by TBS
	    //$tbsProduct->ProductsAux_Voice->ProductsKey = 0; // appears to be assigned by TBS

		return $tbsProduct;
		
	}
}

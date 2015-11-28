<?php
/**
 * @file
 * XS Snapshot Policy entity class.
 */

/**
 * Class XsEntityApiController
 */
class XsEntityApiController extends EntityAPIController {

}

/**
 * Class XsMetadataController
 */
class XsMetadataController extends EntityDefaultMetadataController {
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['name'] = array(
      'label' => t('Policy title'),
      'schema field' => 'name',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('Title of the policy.'),
    );
    $properties['target_id'] = array(
      'label' => t('Parent VM'),
      'schema field' => 'target_id',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('Linking VM node nid'),
    );
    $properties['frequency'] = array(
      'label' => t('Policy type'),
      'schema field' => 'frequency',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('Snapshot policy type'),
    );
    $properties['uid'] = array(
      'label' => t('User id'),
      'schema field' => 'uid',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('User ID who owns the policy'),
    );
    $properties['status'] = array(
      'label' => t('Active'),
      'schema field' => 'status',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('Is the policy active or not'),
    );
    $properties['created'] = array(
      'label' => t('Policy created'),
      'schema field' => 'created',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('created date of the policy.'),
    );
    $properties['changed'] = array(
      'label' => t('Policy updated'),
      'schema field' => 'changed',
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('Updated date of the policy.'),
    );

    return $info;
  }
}
